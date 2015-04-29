
// Perform minimum initialization needed to establish a safe state
void establish_safe_state() {
  // Solenoids
  pinMode(SLND_FRONT_HATCH_PIN, OUTPUT); digitalWrite(SLND_FRONT_HATCH_PIN, LOW);
  pinMode(SLND_LAUNCH_ROD_PIN, OUTPUT); digitalWrite(SLND_LAUNCH_ROD_PIN, LOW);
  pinMode(SLND_ROCKET_HATCH_PIN, OUTPUT); digitalWrite(SLND_ROCKET_HATCH_PIN, LOW);
  pinMode(SLND_ARM_LATCH_PIN, OUTPUT); digitalWrite(SLND_ARM_LATCH_PIN, LOW);



  // DC Motors
  Igniter_Inserter_Motor.setup();
  Elevator_Motor.setup();
  Belt_Linear_Motor.setup();
  Arm_Pitch_Motor.setup();
  Arm_Pitch_Motor.enable();
  Arm_Pitch_Motor.setDirection(PololuDC::DC_BRAKE);


}


void pin_setup() {
  // steppers
  AFMS.begin();
  Arm_Yaw_Stepper->release();
  Nose_Closure_Stepper->release();


  // inputs
  pinMode(LMTS_IGNITER_INSERTER_PIN, INPUT_PULLUP);
  pinMode(LMTS_ELEVATOR_TOP_PIN, INPUT_PULLUP);
  pinMode(LMTS_ELEVATOR_BOTTOM_PIN, INPUT_PULLUP);
  pinMode(LMTS_NOSECONE_CLOSURE_PIN, INPUT_PULLUP);
  pinMode(LMTS_RAKE_NEAR_PIN, INPUT_PULLUP);
  pinMode(LMTS_ARM_PITCH_PIN, INPUT_PULLUP);
  pinMode(LMTS_ARM_YAW_PIN, INPUT_PULLUP);
  pinMode(LMTS_BELT_LINEAR_NEAR_PIN, INPUT_PULLUP);
  pinMode(LMTS_BELT_LINEAR_FAR_PIN, INPUT_PULLUP);

  pinMode(CFSW_PAUSE_ENABLE_PIN, INPUT);

  pinMode(ENCR_BELT_LINEAR_A_PIN, INPUT_PULLUP);
  pinMode(ENCR_BELT_LINEAR_B_PIN, INPUT_PULLUP);



  // outputs
  pinMode(VIND_ASG_PIN, OUTPUT); digitalWrite(VIND_ASG_PIN, LOW);
  pinMode(VIND_PWR_PIN, OUTPUT); digitalWrite(VIND_PWR_PIN, LOW);
}
