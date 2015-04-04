// Configuration files
#include "pin_declaration.h"

// External libraries
#include <avr/wdt.h>

// Internal Libraries
#include "PolledUpdate.h"
#include "PololuDC.h"


// Parameters


// Global variables
unsigned long time;

long encoder_position = 0;
unsigned int encoder_errors = 0;


enum state_t {
  INIT,
  HALT,
  ERROR_STATE,
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
  // IDLE



};

state_t FSM_state = INIT;

unsigned long state_transition_time = 0;


// Threads
#include "Halt.h"
#include "StateMachine.h"
#include "Encoder.h"
#include "Debug.h"
#include "Motion.h"

// Threads
Thread Encoder_Thread(&encoder_cb, 1);
Thread FSM_Thread(&state_machine_cb, 10);
Thread Motion_Thread(&motion_cb, 10); // TODO: what frequency is this!?
Thread Halt_Thread(&halt_cb, 50);
Thread Debug_Thread(&debug_cb, 500);


void setup() {
  establishSafeState(); // Immediately place system into a safe state
  pinSetup();


  Serial.begin(9600);

  waitForEnable();
}

void loop() {
  time = millis();

  Encoder_Thread.update(time);
  FSM_Thread.update(time);
  Halt_Thread.update(time);
  Debug_Thread.update(time);
}

void establishSafeState() {
  // haha nope
}


// Wait until the pause switch has a rising edge
// Requires a toggle when system is initialized in "Enable" position
void waitForEnable() {
  static boolean val = true, prev_val = true;
  boolean wait = true;
  while (wait) {
    if (val && !prev_val) {
      wait = false;
    }
    prev_val = val;
    val = digitalRead(CFSW_PAUSE_ENABLE_PIN);
    delay(50);
  }

}



void pinSetup() {
  pinMode(CFSW_PAUSE_ENABLE_PIN, INPUT);
  pinMode(ENCR_BELT_LINEAR_A_PIN, INPUT_PULLUP);
  pinMode(ENCR_BELT_LINEAR_B_PIN, INPUT_PULLUP);
}

