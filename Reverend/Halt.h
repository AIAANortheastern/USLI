// Temporarily shunt state to pause when pause/enable switch is not enabled
// Restore state to that when the system was paused when pause is disengaged.
void halt_cb(unsigned long diff) {
  bool val = digitalRead(CFSW_PAUSE_ENABLE_PIN);
if (FSM_state != HALT) {
  if (!val) {
    FSM_state = HALT;
  }
}

}
