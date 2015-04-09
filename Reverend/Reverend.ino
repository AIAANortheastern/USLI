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
unsigned long time = 0;

long encoder_position = 0;
unsigned int encoder_errors = 0;


// State machine
enum state_t {
  AWAIT_ENABLE,
  INIT,
  HALT,
  ERROR_STATE,
  COMPLETE,
  HOME_MOTORS,
  RELEASE_DOOR,
  EXTEND_BELT,
  FIND_LINEAR_SETPOINT,
  FIND_YAW_SETPOINT,
  INITIATE_VISION,
  ACCEPT_PACKET,
  REMOTE_MOVE,
  SEND_ACK,
  DRIVE_CONVEYOR,
  RAISE_CONVEYOR,
  STORE_ARM,
  RETRACT_BELT,
  LOWER_ELEVATOR,
  RUN_ELEVATOR,
  AWAIT_PAYLOAD,
  DELAY_NOSE_CLOSURE,
  DEPLOY_NOSE_CLOSURE,
  RETRACT_NOSE_CLOSURE,
  ERECT_ROCKET,
  INSERT_IGNITER
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
Thread DC_Motion_Thread(&dc_motion_cb, 1); // TODO: Decrease update frequency for dc
Thread Halt_Thread(&halt_cb, 50);
Thread Visual_Indicators_Thread(&visual_indicators_cb, 500);
Thread Debug_Thread(&debug_cb, 100);


// Functions
#include "IOSetup.h"

void setup() {
  establish_safe_state(); // Immediately place system into a safe state
  
  pin_setup(); // set up non-critical pins

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
