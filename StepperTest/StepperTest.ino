
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"


Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  Serial.begin(9600);
  Serial.println("Stepper test!");

  AFMS.begin(); 
  myMotor->setSpeed(100);
}

void loop() {
//   myMotor->step(200, BACKWARD, DOUBLE);  // this opens the nosecone
   myMotor->step(200, FORWARD, DOUBLE);  // this closes the nosecone
}
