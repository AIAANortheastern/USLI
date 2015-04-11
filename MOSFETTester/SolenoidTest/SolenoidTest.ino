const int pin = 13;

#define ENCR_BELT_LINEAR_A_PIN 45
#define ENCR_BELT_LINEAR_B_PIN 47

void setup() {
  // put your setup code here, to run once:
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);


pinMode(ENCR_BELT_LINEAR_A_PIN, INPUT_PULLUP);
pinMode(ENCR_BELT_LINEAR_B_PIN, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(13, digitalRead(ENCR_BELT_LINEAR_A_PIN));

digitalWrite(12, digitalRead(ENCR_BELT_LINEAR_B_PIN));



}
