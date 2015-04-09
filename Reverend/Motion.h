void stepper_motion_cb(unsigned long diff) {
  // none
}

void dc_motion_cb(unsigned long diff) {
  // Enforce soft limits
  // TODO: Direction sensitivity
  // TODO: The rest of the motors with limit switches
  boolean near, far;
  far = digitalRead(LMTS_IGNITER_INSERTER_FAR_PIN);
  near = digitalRead(LMTS_IGNITER_INSERTER_NEAR_PIN);
  if (!(far && near)) {
    Igniter_Inserter_Motor.setSpeed(0);
    Igniter_Inserter_Motor.disable();
  }


  far = digitalRead(LMTS_BELT_LINEAR_FAR_PIN);
  near = digitalRead(LMTS_BELT_LINEAR_NEAR_PIN);
  if (!(far && near)) {
    Belt_Linear_Motor.setSpeed(0);
    Belt_Linear_Motor.disable();
  }

}
