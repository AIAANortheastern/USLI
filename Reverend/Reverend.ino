// External libraries
#include <avr/wdt.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Internal Libraries
#include "Thread.h"
#include "PololuDC.h"





// Parameters
#include "PinDefinitions.h"

// Global variables
unsigned long time;

long encoder_position = 0;
unsigned int encoder_errors = 0;


// State machine
enum state_t {
  AWAIT_ENABLE,
  INIT, // start main logic
  HALT,
  ERROR_STATE,
  COMPLETE, // end
  HOME_MOTORS, // multi-sensor trigger
  RELEASE_DOOR, // timed
  INITIATE_VISION, // single
  ACCEPT_PACKET, // trigger
  REMOTE_MOVE, // sensor trigger (?)
  SEND_ACK, // single
  DRIVE_CONVEYOR, // sensor trigger
  RAISE_CONVEYOR, // sensor trigger
  STORE_ARM, // sensor trigger
  RETRACT_BELT, // sensor trigger
  RUN_ELEVATOR, // sensor trigger
  AWAIT_PAYLOAD, // sensor trigger
  DELAY_NOSE_CLOSURE, // timed
  DEPLOY_NOSE_CLOSURE, // count
  RETRACT_NOSE_CLOSURE, // count
  ERECT_ROCKET, // timed
  INSERT_IGNITER // sensor trigger
};

state_t FSM_state = AWAIT_ENABLE;

unsigned long state_transition_time = 0;


// Threads
#include "Halt.h"
#include "StateMachine.h"
#include "Encoder.h"
#include "Debug.h"
#include "Motion.h"
#include "VisualIndicators.h"

// Threads
Thread Encoder_Thread(&encoder_cb, 1);
Thread FSM_Thread(&state_machine_cb, 10);
Thread Stepper_Motion_Thread(&stepper_motion_cb, 1); // TODO: Decrease update frequency for steppers
Thread DC_Motion_Thread(&dc_motion_cb, 10); // TODO: Decrease update frequency for steppers
Thread Halt_Thread(&halt_cb, 50);
Thread Visual_Indicators_Thread(&visual_indicators_cb, 500);
Thread Debug_Thread(&debug_cb, 100);


// Functions
#include "IOSetup.h"

void setup() {

  establishSafeState(); // Immediately place system into a safe state
  
  pinSetup();


  Serial.begin(9600);

}

void loop() {
  time = millis();


  Encoder_Thread.update(time);
  FSM_Thread.update(time);
  Stepper_Motion_Thread.update(time);
  DC_Motion_Thread.update(time);
  Halt_Thread.update(time);
  Visual_Indicators_Thread.update(time);
  Debug_Thread.update(time);
}
