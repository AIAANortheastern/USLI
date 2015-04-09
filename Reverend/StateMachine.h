// Wait for enable switch to start state machine
// Exits on sensor input
void state_await_enable(unsigned long diff, unsigned long state_time) {
  static boolean val = true;
  static boolean prev_val = true;
  static unsigned long latch_time = 0;

  if (state_time - latch_time > 50) {
    if (val && !prev_val) {
      state_transition_time = state_time;
      FSM_state = INIT;
    }

    latch_time = time;
    prev_val = val;
    val = digitalRead(CFSW_PAUSE_ENABLE_PIN);
  }
}

// Pulse the front hatch and rocket door solenoids to open
// Exits after set time
void state_release_doors(unsigned long diff, unsigned long state_time) {
  pinMode(SLND_FRONT_HATCH_PIN, HIGH);
  pinMode(SLND_ROCKET_HATCH_PIN, HIGH);
  if (state_time > 750) {
    pinMode(SLND_FRONT_HATCH_PIN, LOW);
    pinMode(SLND_ROCKET_HATCH_PIN, LOW);
    state_transition_time = time;
    FSM_state = HOME_MOTORS;
  }
}


// Home motors that use encoders and reset integrators
// Exits on multiple sensor inputs
void state_home_motors(unsigned long diff, unsigned long state_time) {
  
  static boolean arm_yaw_homed = false;
  static boolean belt_linear_homed = false;

  if (!arm_yaw_homed) {
    bool limit = digitalRead(LMTS_ARM_YAW_PIN);
    arm_yaw_stepper_target = arm_yaw_stepper_pos - 200; // pull backwards continuously
    if (!limit) { // pressed
      arm_yaw_stepper_pos = 0;
      arm_yaw_stepper_target = 0;
      arm_yaw_homed = true;
    }
  }


  if (!belt_linear_homed) {
    boolean limit = digitalRead(LMTS_BELT_LINEAR_NEAR_PIN); // triggers when LOW
    if (limit) { // not pressed, back up
      Belt_Linear_Motor.enable();
      Belt_Linear_Motor.setDirection(PololuDC::DC_BACKWARD);
      Belt_Linear_Motor.setSpeed(32);
    } else { // hit limit switch
      Belt_Linear_Motor.setDirection(PololuDC::DC_BRAKE);
      belt_linear_homed = true;
      encoder_position = 0;
    }
  }

  if (arm_yaw_homed && belt_linear_homed) {
    state_transition_time = time;
    FSM_state = CENTER_ARM_YAW;
  }

  if (state_time > 15000) {// Timed out! Something is wrong
    Serial.print("[FSM.HOME] Critical error! Motors cannot be homed: ");
    Serial.print(arm_yaw_homed);
    Serial.print(belt_linear_homed);

    state_transition_time = time;
    FSM_state = ERROR_STATE;
  }
}


// Center the arm to scanning position
// Exits after a number of steps
void state_center_arm_yaw(unsigned long diff, unsigned long state_time) {
  arm_yaw_stepper_target = 2550; // 90 degrees
  if (arm_yaw_stepper_target == arm_yaw_stepper_pos) {
    state_transition_time = time;
    FSM_state = INITIATE_VISION;
  }
}


// Send command to RasPi signalling that movement commands will be accepted
// Exits immediately
void state_initiate_vision(unsigned long diff, unsigned long state_time) {
  // Vision system disabled presently
  Serial.println("[FSM] Vision system disabled");
  state_transition_time = time;
  FSM_state = MOVE_BELT_STEPS;
}


// Move forward a specified distance, delay, then repeat
void state_move_belt_steps(unsigned long diff, unsigned long state_time) {
  static unsigned int step_count = 0;
  long target = (step_count + 1) * LINEAR_BELT_STEPS_PER_FOOT * 1.5;
  int velocity = constrain(abs(target - encoder_position), 32, 255);
  if (step_count > 3) {
    Belt_Linear_Motor.setDirection(PololuDC::DC_BRAKE);
    state_transition_time = time;
    FSM_state = ARM_YAW_SETPOINT;
  } else {
    if (encoder_position < target) {
      Belt_Linear_Motor.enable();
      Belt_Linear_Motor.setDirection(PololuDC::DC_BACKWARD);
      Belt_Linear_Motor.setSpeed(velocity);
    } else {
      step_count++;
      Belt_Linear_Motor.setSpeed(0);
      Belt_Linear_Motor.disable();
      state_transition_time = time;
      FSM_state = DELAY_BELT_STEPS;
    }

  }
}


// Wait 10 seconds then shunt back to previous state
void state_delay_belt_steps(unsigned long diff, unsigned long state_time) {
  if (state_time > 10000) {
    state_transition_time = time;
    FSM_state = MOVE_BELT_STEPS;
  }
}


// Rotate the arm yaw stepper to a predefined setpoint
// Exits after stepper is pulsed a set number of times
void state_arm_yaw_setpoint(unsigned long diff, unsigned long state_time) {
  arm_yaw_stepper_target = 3120; // ~110 degrees
  if (arm_yaw_stepper_target == arm_yaw_stepper_pos) {
    state_transition_time = time;
    FSM_state = DROP_CONVEYOR;
  }
}


// Drive conveyor for a specified duration
// Exits on timed transition
void state_drop_conveyor(unsigned long diff, unsigned long state_time) {
  if (state_time < 100) {
    Arm_Pitch_Motor.enable();
    Arm_Pitch_Motor.setDirection(PololuDC::FORWARD);
    Arm_Pitch_Motor.setSpeed(128);
  } else {
    Arm_Pitch_Motor.setDirection(PololuDC::RELEASE);
  }

  if (state_time > 2100) {
    state_transition_time = time;
    FSM_state = RUN_RAKES;
  }
}



void state_run_rakes(unsigned long diff, unsigned long state_time) {
  boolean val = digitalRead(LMTS_RAKE_NEAR_PIN);
  if (val) { // not pressed, back up
    Arm_Pitch_Motor.enable();
    Arm_Pitch_Motor.setDirection(PololuDC::FORWARD);
    Arm_Pitch_Motor.setSpeed(64);
  } else { // pressed, stop
    Arm_Pitch_Motor.setDirection(PololuDC::BRAKE);
    state_transition_time = time;
    FSM_state = DELAY_POST_RAKE;
  }
}


void state_delay_post_rake(unsigned long diff, unsigned long state_time) {
  if (state_time > 4000) {
    state_transition_time = time;
    FSM_state = BRING_ARM_UP;
  }
}


// stop on limit switch
void state_bring_arm_up(unsigned long diff, unsigned long state_time) {
  boolean val = digitalRead(LMTS_ARM_PITCH_PIN);

  if (val) { // not pressed
    int velocity = max(32, 255 - state_time);
    Arm_Pitch_Motor.enable();
    Arm_Pitch_Motor.setDirection(PololuDC::FORWARD);
    Arm_Pitch_Motor.setSpeed(velocity);
  } else { // pressed
    Arm_Pitch_Motor.setDirection(PololuDC::BRAKE);
    state_transition_time = time;
    FSM_state = PERP_ARM;
  }
}





void state_perp_arm(unsigned long diff, unsigned long state_time) {
  arm_yaw_stepper_target = 2550; // 90 degrees
  if (arm_yaw_stepper_target == arm_yaw_stepper_pos) {
    state_transition_time = time;
    FSM_state = ZERO_BELT;
  }
}



// Bring the belt back to the home position

void state_zero_belt(unsigned long diff, unsigned long state_time) {
  int velocity = constrain(encoder_position, 32, 255);
  boolean limit = digitalRead(LMTS_BELT_LINEAR_NEAR_PIN);
  if (limit) { // not pressed
    Belt_Linear_Motor.enable();
    Belt_Linear_Motor.setDirection(PololuDC::DC_BACKWARD);
    Belt_Linear_Motor.setSpeed(velocity);
  } else { // pressed
    Belt_Linear_Motor.setDirection(PololuDC::DC_BRAKE);
    state_transition_time = time;
    FSM_state = DROP_ELEVATOR;
  }
}



void state_drop_elevator(unsigned long diff, unsigned long state_time) {
  boolean limit = digitalRead(LMTS_ELEVATOR_BOTTOM_PIN);
  if (limit) { // not pressed
    Elevator_Motor.enable();
    Elevator_Motor.setDirection(PololuDC::DC_BACKWARD);
    Elevator_Motor.setSpeed(32);
  } else { // pressed
    Elevator_Motor.setDirection(PololuDC::DC_BRAKE);
    state_transition_time = time;
    FSM_state = DELAY_ELEVATOR;
  }

}

void state_delay_elevator(unsigned long diff, unsigned long state_time) {
  if (state_time > 5000) {
    state_transition_time = time;
    FSM_state = RAISE_ELEVATOR;
  }
}

void state_raise_elevator(unsigned long diff, unsigned long state_time) {
  boolean limit = digitalRead(LMTS_ELEVATOR_TOP_PIN);
  if (limit) { // not pressed
    Elevator_Motor.enable();
    Elevator_Motor.setDirection(PololuDC::DC_BACKWARD);
    Elevator_Motor.setSpeed(64);
  } else { // pressed
    Elevator_Motor.setDirection(PololuDC::DC_BRAKE);
    state_transition_time = time;
    FSM_state = DELAY_POST_ELEVATOR;
  }
}


void state_delay_post_elevator(unsigned long diff, unsigned long state_time) {
  if (state_time > 10000) {
    state_transition_time = time;
    FSM_state = NOSECONE_CLOSE;
  }
}


void state_nosecone_close(unsigned long diff, unsigned long state_time) {

  nose_closure_stepper_target = nose_closure_stepper_pos + 200; // push forward continuously
  bool limit = digitalRead(LMTS_NOSECONE_CLOSURE_PIN);

  if (!limit) { // pressed
    nose_closure_stepper_target = nose_closure_stepper_pos; // halt motor
    state_transition_time = time;
    FSM_state = NOSECONE_OPEN;
  }
}

void state_nosecone_open(unsigned long diff, unsigned long state_time) {
  nose_closure_stepper_target = 0;
  if (nose_closure_stepper_target == nose_closure_stepper_pos) {
    state_transition_time = time;
    FSM_state = DEPLOY_LAUNCH_RAIL;
  }
}

void state_deploy_launch_rail(unsigned long diff, unsigned long state_time) {
  pinMode(SLND_LAUNCH_ROD_PIN, HIGH);
  if (state_time > 750) {
    pinMode(SLND_LAUNCH_ROD_PIN, LOW);
    state_transition_time = time;
    FSM_state = DELAY_POST_LAUNCH_RAIL;
  }
}

void state_delay_post_launch_rail(unsigned long diff, unsigned long state_time) {
  if (state_time > 15000) {
    state_transition_time = time;
    FSM_state = INSERT_IGNITER;
  }
}

void state_insert_igniter(unsigned long diff, unsigned long state_time) {
  boolean limit = digitalRead(LMTS_IGNITER_INSERTER_PIN);
  if (limit) { // not pressed
    Igniter_Inserter_Motor.enable();
    Igniter_Inserter_Motor.setDirection(PololuDC::DC_FORWARD);
    Igniter_Inserter_Motor.setSpeed(64);
  } else { // pressed
    Igniter_Inserter_Motor.setDirection(PololuDC::DC_BRAKE);
    state_transition_time = time;
    FSM_state = COMPLETE;
  }
}




void state_machine_cb(unsigned long diff) {
  switch (FSM_state) {
    case AWAIT_ENABLE:
      state_await_enable(diff, time - state_transition_time);
      break;
    case INIT:
      state_transition_time = time;
      FSM_state = RELEASE_DOORS;
      break;
    case HALT:
      Arm_Yaw_Stepper->release();
      Nose_Closure_Stepper->release();
      establish_safe_state();
      // Do nothing
      break;
    case ERROR_STATE:
      Arm_Yaw_Stepper->release();
      Nose_Closure_Stepper->release();
      establish_safe_state();
      break;
    case HOME_MOTORS:
      state_home_motors(diff, time - state_transition_time);
      break;
    case RELEASE_DOORS:
      state_release_doors(diff, time - state_transition_time);
      break;
    case CENTER_ARM_YAW:
      state_center_arm_yaw(diff, time - state_transition_time);
      break;
    case INITIATE_VISION:
      state_initiate_vision(diff, time - state_transition_time);
      break;
    case MOVE_BELT_STEPS:
      state_move_belt_steps(diff, time - state_transition_time);
      break;
    case DELAY_BELT_STEPS:
      state_delay_belt_steps(diff, time - state_transition_time);
      break;
    case ARM_YAW_SETPOINT:
      state_arm_yaw_setpoint(diff, time - state_transition_time);
      break;
    case DROP_CONVEYOR:
      state_drop_conveyor(diff, time - state_transition_time);
      break;
    case RUN_RAKES:
      state_run_rakes(diff, time - state_transition_time);
      break;
    case DELAY_POST_RAKE:
      state_delay_post_rake(diff, time - state_transition_time);
      break;
    case BRING_ARM_UP:
      state_bring_arm_up(diff, time - state_transition_time);
      break;
    case PERP_ARM:
      state_perp_arm(diff, time - state_transition_time);
      break;
    case ZERO_BELT:
      state_zero_belt(diff, time - state_transition_time);
      break;
    case DROP_ELEVATOR:
      state_drop_elevator(diff, time - state_transition_time);
      break;
    case DELAY_ELEVATOR:
      state_delay_elevator(diff, time - state_transition_time);
      break;
    case RAISE_ELEVATOR:
      state_raise_elevator(diff, time - state_transition_time);
      break;
    case DELAY_POST_ELEVATOR:
      state_delay_post_elevator(diff, time - state_transition_time);
      break;
    case NOSECONE_CLOSE:
      state_nosecone_close(diff, time - state_transition_time);
      break;
    case NOSECONE_OPEN:
      state_nosecone_open(diff, time - state_transition_time);
      break;
    case DEPLOY_LAUNCH_RAIL:
      state_deploy_launch_rail(diff, time - state_transition_time);
      break;
    case DELAY_POST_LAUNCH_RAIL:
      state_delay_post_launch_rail(diff, time - state_transition_time);
      break;
    case INSERT_IGNITER:
      state_insert_igniter(diff, time - state_transition_time);
      break;
    case COMPLETE:
      // wait forever
      break;
    default:
      Serial.print("[FSM] Critical error! State {");
      Serial.print(FSM_state);
      Serial.println("} not defined! Halting...");
      state_transition_time = time;
      FSM_state = ERROR_STATE;
  }
}




