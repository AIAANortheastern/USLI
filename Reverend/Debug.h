void debug_cb(unsigned long diff) {
  // nothing
  Serial.print("Diagnostics > State: ");
  Serial.print(FSM_state);
  Serial.print("\tEncoder Position: ");
  Serial.print(encoder_position);
  Serial.print("\t Errors: ");
  Serial.println(encoder_errors);
}
