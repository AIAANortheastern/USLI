

#include <Wire.h>
#include <Adafruit_MotorShield.h>


Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_StepperMotor *motorA = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *motorB = AFMS.getStepper(200, 2);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz

  motorA->setSpeed(200); 
  motorB->setSpeed(200); 
  motorA->step(10, FORWARD, SINGLE);
}

void loop() {
  Serial.println("Single coil steps");

//motorA->release();
  motorA->step(200, FORWARD, DOUBLE ); 
//  motorB->release();


}
