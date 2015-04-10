

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"


Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *motorA = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *motorB = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *myMotor;

char cmd = 'x';

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  motorA->setSpeed(100);
  motorB->setSpeed(100);
  myMotor = motorA;
}

void loop() {
  while (Serial.available()) {
    cmd = Serial.read();
    if (cmd == 'f') {
      Serial.println("Forward");
    } else if (cmd == 'b') {
      Serial.println("Backward");
    } else if (cmd == '1') {
      Serial.println("Motor 1 Selected");
      myMotor = motorA;
      motorB->release();
    } else if (cmd == '2') {
      Serial.println("Motor 2 Selected");
      myMotor = motorB;
      motorA->release();
    } else {
      Serial.println("Halt");
    }
  }

  if (cmd == 'f') {
    myMotor->step(10, FORWARD, DOUBLE);  // this closes the nosecone
  }

  if (cmd == 'b') {
    myMotor->step(10, BACKWARD, DOUBLE);  // this opens the nosecone
  }
}
