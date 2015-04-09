#define VIND_ASG_PIN 12
#define VIND_PWR_PIN 13

#define SLND_FRONT_HATCH_PIN 22
#define SLND_LAUNCH_ROD_PIN 24
#define SLND_ROCKET_HATCH_PIN 26



void setup() {
  // put your setup code here, to run once:
  pinMode(SLND_FRONT_HATCH_PIN, OUTPUT); digitalWrite(SLND_FRONT_HATCH_PIN, LOW);
  pinMode(SLND_LAUNCH_ROD_PIN, OUTPUT); digitalWrite(SLND_LAUNCH_ROD_PIN, LOW);
  pinMode(SLND_ROCKET_HATCH_PIN, OUTPUT); digitalWrite(SLND_ROCKET_HATCH_PIN, LOW);
  pinMode(VIND_ASG_PIN, OUTPUT); digitalWrite(VIND_ASG_PIN, LOW);
  pinMode(VIND_PWR_PIN, OUTPUT); digitalWrite(VIND_PWR_PIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
 digitalWrite(VIND_ASG_PIN, LOW);
 digitalWrite(VIND_PWR_PIN, LOW);
 digitalWrite(SLND_FRONT_HATCH_PIN, LOW);
  digitalWrite(SLND_LAUNCH_ROD_PIN, LOW);
   digitalWrite(SLND_ROCKET_HATCH_PIN, LOW);
 delay(1000);
  digitalWrite(VIND_ASG_PIN, HIGH);
 digitalWrite(VIND_PWR_PIN, LOW);
  digitalWrite(SLND_FRONT_HATCH_PIN, HIGH);
  digitalWrite(SLND_LAUNCH_ROD_PIN, LOW);
   digitalWrite(SLND_ROCKET_HATCH_PIN, LOW);
 delay(1000);
  digitalWrite(VIND_ASG_PIN, HIGH);
 digitalWrite(VIND_PWR_PIN, HIGH);
  digitalWrite(SLND_FRONT_HATCH_PIN, LOW);
  digitalWrite(SLND_LAUNCH_ROD_PIN, HIGH);
   digitalWrite(SLND_ROCKET_HATCH_PIN, LOW);
 delay(1000);
  digitalWrite(VIND_ASG_PIN, LOW);
 digitalWrite(VIND_PWR_PIN, HIGH);
  digitalWrite(SLND_FRONT_HATCH_PIN, LOW);
  digitalWrite(SLND_LAUNCH_ROD_PIN, LOW);
   digitalWrite(SLND_ROCKET_HATCH_PIN, HIGH);
 delay(1000);
}
