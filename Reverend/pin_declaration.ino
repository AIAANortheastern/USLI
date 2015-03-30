// PRELIMINARY ONLY!
// CHECK PHYSICAL CONSIDERATIONS

/* Abbreviations:
DCMD - DC Motor Driver
VIND - Visual Indicator Light
SLND - Solenoid
LMTS - Limit Switch
POTI - Potentiometer
VMON - Voltage Monitor
BBRK - Beam Break Sensor
*/

#define DCMD_IGNITER_INSERTER_INA_PIN 27
#define DCMD_IGNITER_INSERTER_INB_PIN 28
#define DCMD_IGNITER_INSERTER_PWM_PIN 2
#define DCMD_IGNITER_INSERTER_DGN_PIN 29
#define DCMD_IGNITER_INSERTER_CS_PIN A12

#define DCMD_ELEVATOR_INA_PIN 30
#define DCMD_ELEVATOR_INB_PIN 31
#define DCMD_ELEVATOR_PWM_PIN 3
#define DCMD_ELEVATOR_DGN_PIN 32
#define DCMD_ELEVATOR_CS_PIN A13

#define DCMD_NOSECONE_CLOSURE_INA_PIN 33
#define DCMD_NOSECONE_CLOSURE_INB_PIN 34
#define DCMD_NOSECONE_CLOSURE_PWM_PIN 4
#define DCMD_NOSECONE_CLOSURE_DGN_PIN 35
#define DCMD_NOSECONE_CLOSURE_CS_PIN A14

#define DCMD_ARM_PITCH_INA_PIN 36
#define DCMD_ARM_PITCH_INB_PIN 37
#define DCMD_ARM_PITCH_PWM_PIN 5
#define DCMD_ARM_PITCH_DGN_PIN 38
#define DCMD_ARM_PITCH_CS_PIN A15


#define VIND_ASG_PIN 6
#define VIND_ERR_PIN 7


#define SLND_FRONT_HATCH_A_PIN 22
#define SLND_FRONT_HATCH_B_PIN 23
#define SLND_LAUNCH_ROD_PIN 24
#define SLND_ROCKET_HATCH_A_PIN 25
#define SLND_ROCKET_HATCH_B_PIN 26


#define LMTS_IGNITER_INSERTER_PIN 39
#define LMTS_ELEVATOR_PIN 40
#define LMTS_NOSECONE_CLOSURE_PIN 41
#define LMTS_ARM_PITCH_PIN 42
#define LMTS_ARM_YAW_PIN 43
#define LMTS_BELT_LINEAR_A_PIN 44
#define LMTS_BELT_LINEAR_B_PIN 45


#define POTI_ARM_PITCH_PIN A0
#define POTI_ARM_YAW_PIN A1


#define VMON_PIN A2


#define BBRK_ROCKET_INSERTION_PIN A9
#define BBRK_RAMP_CAPTURE_PIN A10
#define BBRK_ELEVATOR_INSERTION_PIN A11
