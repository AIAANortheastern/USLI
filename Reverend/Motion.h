// Must be called every 3ms for optimal speed!
void stepper_motion_cb(unsigned long diff) {
  // Arm Yaw
  if (arm_yaw_stepper_target > arm_yaw_stepper_pos) {
    Arm_Yaw_Stepper.onestep(FORWARD, DOUBLE);
    arm_yaw_stepper_pos++;
  }
  if (arm_yaw_stepper_target < arm_yaw_stepper_pos) {
    Arm_Yaw_Stepper.onestep(BACKWARD, DOUBLE);
    arm_yaw_stepper_pos--;
  }

  // Nose Closure
  if (nose_closure_stepper_target > nose_closure_stepper_pos) {
    Nose_Closure_Stepper.onestep(FORWARD, DOUBLE);
    nose_closure_stepper_pos++;
  }
  if (nose_closure_stepper_target < nose_closure_stepper_pos) {
    Nose_Closure_Stepper.onestep(BACKWARD, DOUBLE);
    nose_closure_stepper_pos--;
  }
  if (nose_closure_stepper_target == nose_closure_stepper_pos) { // disengage nose closure since no torque is needed
    Nose_Closure_Stepper.release();
  }
}

void dc_motion_cb(unsigned long diff) {
 /* WARNING! SAFETY IS DISABLED! */
}

// TODO: Halt motors, etc when halted or error
