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
  INSERT_IGNITER // sensor trigger
  // IDLE
};
*/


void state_machine_cb(unsigned long diff) {
  switch (FSM_state) {
    case INIT:
      transition_time = time;
      // don't start yet
      break;
    case HALT:
      // do nothing
      break;
    case ERROR_STATE:
      // also do nothing
      break;
    case HOME_MOTORS:
        state_home_motor(diff, time - transition_time);
      break;
    default:
      Serial.print("[FSM] Critical error! State ");
      Serial.print(FSM_state);
      Serial.println(" not defined! Halting...");
      transition_time = time;
      FSM_state = ERROR_STATE;
  }
}

void state_home_motor(unsigned long diff, unsigned long state_time) {
  
}

void state_release_door(unsigned long diff, unsigned long state_time) {
  
}

void state_initiate_vision(unsigned long diff, unsigned long state_time) {
  
}

void 
