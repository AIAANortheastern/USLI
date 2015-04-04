

// Wait for enable switch to start state machine
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


// Home motors that use encoders and reset integrators
void state_home_motor(unsigned long diff, unsigned long state_time) {
  static unsigned char phase = 0;

  // TODO: home motors
  // currently bypassing directly to the door release
  Serial.println("Bypassing motor homing routine...");
  state_transition_time = time;
  FSM_state = RELEASE_DOOR;
}

// Pulse the front hatch and rocket door solenoids to open
void state_release_door(unsigned long diff, unsigned long state_time) {
  pinMode(SLND_FRONT_HATCH_PIN, HIGH);
  pinMode(SLND_ROCKET_HATCH_PIN, HIGH);
  if (state_time > 750) { // TODO: needs adjusted timing for nichrome burners
    pinMode(SLND_FRONT_HATCH_PIN, LOW);
    pinMode(SLND_ROCKET_HATCH_PIN, LOW);
    state_transition_time = time;
    FSM_state = INITIATE_VISION;
  }
}

// Send command to RasPi signalling that movement commands will be accepted
void state_initiate_vision(unsigned long diff, unsigned long state_time) {
  // TODO: write this function
  Serial.println("Bypassing vision initialisation routine...");
  state_transition_time = time;
  FSM_state = ACCEPT_PACKET;
}

// Process packets from the RasPi and delegate to relevant states and functions
void state_accept_packet(unsigned long diff, unsigned long state_time) {
  // TODO: write this
  Serial.println("Bypassing vision packet socket loop...");
  state_transition_time = time;
  FSM_state = DRIVE_CONVEYOR;

}

// Initiate requested motion
void state_remote_move(unsigned long diff, unsigned long state_time) {
  // TODO: write this
}

// Signal to RasPi that requested action is complete.
void state_send_ack(unsigned long diff, unsigned long state_time) {
  // TODO: write
   state_transition_time = time;
  FSM_state = ACCEPT_PACKET;
}

// Drive conveyor until beam break is triggered
void state_drive_conveyor(unsigned long diff, unsigned long state_time) {
  // TODO: write
  Serial.println("Bypassing conveyor drive...");
  state_transition_time = time;
  FSM_state = RAISE_CONVEYOR;
}

// Raise conveyor until limit switch is triggered
void state_raise_conveyor(unsigned long diff, unsigned long state_time) {
  // TODO: this
  Serial.println("Bypassing conveyor raise...");
  state_transition_time = time;
  FSM_state = STORE_ARM;
}

// Rotate arm yaw until limit switch is triggered
void state_store_arm(unsigned long diff, unsigned long state_time) {
  // TODO: this
  Serial.println("Bypassing arm stow...");
  state_transition_time = time;
  FSM_state = RETRACT_BELT;
}


// Retract linear belt until limit switch is triggered (can ignore encoder)
void state_retract_belt(unsigned long diff, unsigned long state_time) {
  // todo: this is already written somewhere?
  Serial.println("Bypassing belt retract...");
  state_transition_time = time;
  FSM_state = RUN_ELEVATOR;
}


// Raise elevator until beam break is triggered
void state_run_elevator(unsigned long diff, unsigned long state_time) {
  // TODO: yeah
  Serial.println("Bypassing elevator run...");
  state_transition_time = time;
  FSM_state = AWAIT_PAYLOAD;

}

// Await beam break indicating successful payload capture.
// Time out to a failure state indicating a lost payload
void state_await_payload(unsigned long diff, unsigned long state_time) {
  // TODO: yeah
  Serial.println("Bypassing payload wait procedure...");
  state_transition_time = time;
  FSM_state = DELAY_NOSE_CLOSURE;
}

// Wait for 5 seconds for payload to settle
void state_delay_nose_closure(unsigned long diff, unsigned long state_time) {
  if (state_time > 5000) {
    state_transition_time = time;
    FSM_state = DEPLOY_NOSE_CLOSURE;
  }
}

// Run nosecone closure motor until limit switch is triggered.
// Record and store the number of steps needed to do so.
void state_deploy_nose_closure(unsigned long diff, unsigned long state_time) {
  // TODO: yeah
  Serial.println("Bypassing nose closure deployment procedure...");
  state_transition_time = time;
  FSM_state = RETRACT_NOSE_CLOSURE;
}

// Retract stepper by the number of steps recorded by the previous state
void state_retract_nose_closure(unsigned long diff, unsigned long state_time) {
  // TODO: yeah
  Serial.println("Bypassing nose closure retraction procedure...");
  state_transition_time = time;
  FSM_state = ERECT_ROCKET;
}


// Pulse rocket lift solenoids to engage gas springs
void state_erect_rocket(unsigned long diff, unsigned long state_time) {
  pinMode(SLND_LAUNCH_ROD_PIN, HIGH);
  if (state_time > 750) {
    pinMode(SLND_LAUNCH_ROD_PIN, LOW);
    state_transition_time = time;
    FSM_state = INSERT_IGNITER;
  }

}

// Run steppers until limit 
void state_insert_igniter(unsigned long diff, unsigned long state_time) {
  // TODO: yeah
  Serial.println("Bypassing igniter insertion...");
  state_transition_time = time;
  FSM_state = COMPLETE;
}



void state_machine_cb(unsigned long diff) {
  switch (FSM_state) {
    case AWAIT_ENABLE:
      state_await_enable(diff, time - state_transition_time);
      break;
    case INIT:
      state_transition_time = time;
      FSM_state = HOME_MOTORS;

      break;
    case HALT:
      // do nothing
      break;
    case ERROR_STATE:
      // Remain in error state
      break;
    case HOME_MOTORS:
      state_home_motor(diff, time - state_transition_time);
      break;
    case RELEASE_DOOR:
      state_release_door(diff, time - state_transition_time);
      break;
    case INITIATE_VISION:
      state_initiate_vision(diff, time - state_transition_time);
      break;
    case ACCEPT_PACKET:
      state_accept_packet(diff, time - state_transition_time);
      break;
    case REMOTE_MOVE:
      state_remote_move(diff, time - state_transition_time);
      break;
    case SEND_ACK:
      state_send_ack(diff, time - state_transition_time);
      break;
    case DRIVE_CONVEYOR:
      state_drive_conveyor(diff, time - state_transition_time);
      break;
    case RAISE_CONVEYOR:
      state_raise_conveyor(diff, time - state_transition_time);
      break;
    case STORE_ARM:
      state_store_arm(diff, time - state_transition_time);
      break;
    case RETRACT_BELT:
      state_retract_belt(diff, time - state_transition_time);
      break;
    case RUN_ELEVATOR:
      state_run_elevator(diff, time - state_transition_time);
      break;
    case AWAIT_PAYLOAD:
      state_await_payload(diff, time - state_transition_time);
      break;
    case DELAY_NOSE_CLOSURE:
      state_delay_nose_closure(diff, time - state_transition_time);
      break;
    case DEPLOY_NOSE_CLOSURE:
      state_deploy_nose_closure(diff, time - state_transition_time);
      break;
    case RETRACT_NOSE_CLOSURE:
      state_retract_nose_closure(diff, time - state_transition_time);
      break;
    case ERECT_ROCKET:
      state_erect_rocket(diff, time - state_transition_time);
      break;
    case INSERT_IGNITER:
      state_insert_igniter(diff, time - state_transition_time);
      break;
    case COMPLETE:
      // wait
      break;
    default:
      Serial.print("[FSM] Critical error! State ");
      Serial.print(FSM_state);
      Serial.println(" not defined! Halting...");
      state_transition_time = time;
      FSM_state = ERROR_STATE;
  }
}




