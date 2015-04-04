/*
enum state_t {
 INIT,
 HALT,
 ERROR_STATE,
 HOME_MOTORS, // multi-sensor trigger
 RELEASE_DOOR, // timed
 INITIATE_VISION, // single
 ACCEPT_PACKET, // trigger
 REMOTE_MOVE, // sensor trigger (?)
 SEND_ACK, // single
 DRIVE_CONVEYOR, // sensor trigger
 RAISE_CONVEYOR, // sensor trigger
 STORE_ARM, // sensor trigger
 RETRACT_BELT, // sensor trigger
 RUN_ELEVATOR, // sensor trigger
 AWAIT_PAYLOAD, // sensor trigger
 DELAY_NOSE_CLOSURE, // timed
 DEPLOY_NOSE_CLOSURE, // count
 RETRACT_NOSE_CLOSURE, // count
 ERECT_ROCKET, // timed
 INSERT_IGNITER, // sensor trigger
 COMPLETE // end
 // IDLE
 };
 */


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


void state_home_motor(unsigned long diff, unsigned long state_time) {
  static unsigned char phase = 0;

  // TODO: home motors
  // currently bypassing directly to the door release
  Serial.println("Bypassing motor homing routine...");
  state_transition_time = time;
  FSM_state = RELEASE_DOOR;
}

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

void state_initiate_vision(unsigned long diff, unsigned long state_time) {
  // TODO: write this function
  Serial.println("Bypassing vision initialisation routine...");
  state_transition_time = time;
  FSM_state = ACCEPT_PACKET;
}

void state_accept_packet(unsigned long diff, unsigned long state_time) {
  // TODO: write this
  Serial.println("Bypassing vision packet socket loop...");
  state_transition_time = time;
  FSM_state = DRIVE_CONVEYOR;

}

void state_remote_move(unsigned long diff, unsigned long state_time) {
  // TODO: write this
}

void state_send_ack(unsigned long diff, unsigned long state_time) {
  // TODO: write
}

void state_drive_conveyor(unsigned long diff, unsigned long state_time) {
  // TODO: write
  Serial.println("Bypassing conveyor drive...");
  state_transition_time = time;
  FSM_state = RAISE_CONVEYOR;
}

void state_raise_conveyor(unsigned long diff, unsigned long state_time) {
  // TODO: this
  Serial.println("Bypassing conveyor raise...");
  state_transition_time = time;
  FSM_state = STORE_ARM;
}

void state_store_arm(unsigned long diff, unsigned long state_time) {
  // TODO: this
  Serial.println("Bypassing arm stow...");
  state_transition_time = time;
  FSM_state = RETRACT_BELT;
}

void state_retract_belt(unsigned long diff, unsigned long state_time) {
  // todo: this is already written somewhere?
  Serial.println("Bypassing belt retract...");
  state_transition_time = time;
  FSM_state = RUN_ELEVATOR;
}

void state_run_elevator(unsigned long diff, unsigned long state_time) {
  // TODO: yeah
  Serial.println("Bypassing elevator run...");
  state_transition_time = time;
  FSM_state = AWAIT_PAYLOAD;

}

void state_await_payload(unsigned long diff, unsigned long state_time) {
  // TODO: yeah
  Serial.println("Bypassing payload wait procedure...");
  state_transition_time = time;
  FSM_state = DELAY_NOSE_CLOSURE;
}

void state_delay_nose_closure(unsigned long diff, unsigned long state_time) {
  if (state_time > 5000) {
    state_transition_time = time;
    FSM_state = DEPLOY_NOSE_CLOSURE;
  }
}

void state_deploy_nose_closure(unsigned long diff, unsigned long state_time) {
  // TODO: yeah
  Serial.println("Bypassing nose closure deployment procedure...");
  state_transition_time = time;
  FSM_state = RETRACT_NOSE_CLOSURE;
}

void state_retract_nose_closure(unsigned long diff, unsigned long state_time) {
  // TODO: yeah
  Serial.println("Bypassing nose closure retraction procedure...");
  state_transition_time = time;
  FSM_state = ERECT_ROCKET;
}

void state_erect_rocket(unsigned long diff, unsigned long state_time) {
  pinMode(SLND_LAUNCH_ROD_PIN, HIGH);
  if (state_time > 750) {
    pinMode(SLND_LAUNCH_ROD_PIN, LOW);
    state_transition_time = time;
    FSM_state = INSERT_IGNITER;
  }

}

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




