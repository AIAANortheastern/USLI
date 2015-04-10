void setup() {
  // put your setup code here, to run once:
  for (int i = 35; i < 47; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 35; i < 47; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(digitalRead(i));
    Serial.print('\t');
  }
  Serial.println();
  delay(100);
}


/*
35: igniter inserter
37: elevator bottom
39: rake near
40: arm yaw
41: belt far
42: arm lift
43: belt near
44: nosecone far
46: elevayor top





*/
