void debug_cb(unsigned long diff) {
  // nothing
  Serial.print("Diagnostics > State: ");
  Serial.print(FSM_state);
  Serial.print("\tEncoder Position: ");
  Serial.print(encoder_position);
  Serial.print("\tErrors: ");
  Serial.println(encoder_errors);
  Serial.print("\tYaw Target:");
  Serial.print(arm_yaw_stepper_target);
  Serial.print("\tPos:");
  Serial.print(arm_yaw_stepper_pos);
    Serial.print("\tNose Target:");
  Serial.print(nose_closure_stepper_target);
  Serial.print("\tPos:");
  Serial.print(nose_closure_stepper_pos);
}
