void visual_indicators_cb(unsigned long diff) {
  static boolean state = false;

  if ((FSM_state == AWAIT_ENABLE) || (FSM_state == HALT)) {
    digitalWrite(VIND_PWR_PIN, HIGH);
  } else {
    state ^= true;
    digitalWrite(VIND_PWR_PIN, state);
  }
  
  if (FSM_state == COMPLETE) {
    digitalWrite(VIND_ASG_PIN, HIGH);
  }

}
