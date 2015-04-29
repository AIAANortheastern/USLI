class PololuDC {
  private:
    uint8_t IN_A_PIN;
    uint8_t IN_B_PIN;
    uint8_t PWM_PIN;
    uint8_t DIAG_PIN;
    uint8_t CS_PIN;


  public:
    static const uint8_t DC_FORWARD = 1;
    static const uint8_t DC_BACKWARD = 2;
    static const uint8_t DC_BRAKE = 3;
    static const uint8_t DC_RELEASE = 4;

    PololuDC(uint8_t in_a, uint8_t in_b, uint8_t pwm, uint8_t diag, uint8_t cs) {
      IN_A_PIN = in_a;
      IN_B_PIN = in_b;
      PWM_PIN = pwm;
      DIAG_PIN = diag;
      CS_PIN = cs;
    }

    PololuDC(uint8_t in_a, uint8_t in_b, uint8_t pwm, uint8_t diag) {
      IN_A_PIN = in_a;
      IN_B_PIN = in_b;
      PWM_PIN = pwm;
      DIAG_PIN = diag;
      CS_PIN = -1;
    }


    void setup() {
      pinMode(IN_A_PIN, OUTPUT);
      pinMode(IN_B_PIN, OUTPUT);
      pinMode(PWM_PIN, OUTPUT);
      pinMode(DIAG_PIN, INPUT);
      if (CS_PIN != -1) {
        pinMode(CS_PIN, INPUT);
      }
      disable();
    }

    void setDirection(int8_t dir) {
      if (dir == DC_FORWARD) {
        digitalWrite(IN_A_PIN, HIGH);
        digitalWrite(IN_B_PIN, LOW);
      }
      else if (dir == DC_BACKWARD) {
        digitalWrite(IN_A_PIN, LOW);
        digitalWrite(IN_B_PIN, HIGH);
      }
      else if (dir == DC_BRAKE) {
        digitalWrite(IN_A_PIN, LOW); // TODO: Are these low or high?
        digitalWrite(IN_B_PIN, LOW);
        digitalWrite(PWM_PIN, HIGH);
      }
      else if (dir == DC_RELEASE) {
        digitalWrite(IN_A_PIN, LOW);
        digitalWrite(IN_B_PIN, LOW);
        digitalWrite(PWM_PIN, LOW);
      }
    }

    void setSpeed(uint8_t speed) {
      analogWrite(PWM_PIN, speed);
    }

    float getAmperage() {
      if (CS_PIN == -1) {
        return 0;
      }
      return (float)analogRead(CS_PIN) * 0.03756; // steps * 5 / 1024 / 0.13
    }

    boolean getDiagnosticStatus() {
      return digitalRead(DIAG_PIN);
    }

    void enable() {
      digitalWrite(DIAG_PIN, LOW);
      pinMode(DIAG_PIN, INPUT);
    }

    void disable() {
      pinMode(DIAG_PIN, OUTPUT);
      digitalWrite(DIAG_PIN, LOW);
      digitalWrite(IN_A_PIN, LOW);
      digitalWrite(IN_B_PIN, LOW);
      digitalWrite(PWM_PIN, LOW);
    }

    void safety() {
      Serial.println("[DC] WARNING! OVERLOAD PREVENTION DISABLED!");
      /*
      if (getAmperage() > 18.0) {
        disable();
      }
      */
    }


};

#define SLND_ARM_LATCH_PIN 51


#define DCMD_ARM_PITCH_INA_PIN 24
#define DCMD_ARM_PITCH_INB_PIN 26
#define DCMD_ARM_PITCH_PWM_PIN 2
#define DCMD_ARM_PITCH_DGN_PIN 22


PololuDC Arm_Pitch_Motor(DCMD_ARM_PITCH_INA_PIN, DCMD_ARM_PITCH_INB_PIN, DCMD_ARM_PITCH_PWM_PIN, DCMD_ARM_PITCH_DGN_PIN);

void setup() {
  pinMode(SLND_ARM_LATCH_PIN, OUTPUT); digitalWrite(SLND_ARM_LATCH_PIN, LOW);
  Arm_Pitch_Motor.setup();
  Arm_Pitch_Motor.enable();
  Arm_Pitch_Motor.setDirection(PololuDC::DC_BRAKE);


  delay(1000);
  digitalWrite(SLND_ARM_LATCH_PIN, HIGH);
  delay(200);
  Arm_Pitch_Motor.setDirection(PololuDC::DC_BACKWARD);
  Arm_Pitch_Motor.setSpeed(128);
  delay(100);
  Arm_Pitch_Motor.setDirection(PololuDC::DC_RELEASE);
  delay(500);
  Arm_Pitch_Motor.setDirection(PololuDC::DC_BRAKE);
 
  digitalWrite(SLND_ARM_LATCH_PIN, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

}