#include <Arduino.h>

#define DIR_PIN 5
#define STEP_PIN 6

  int direction = 1; // 1 for clockwise, 0 for counterclockwise



void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Setting up...");

  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
}

void loop() {
  while (Serial.available()) {
    Serial.read(); // Read the incoming byte
    if (direction == 'd') {
      direction = 0; // Change to counterclockwise
      Serial.println("Changing direction to counterclockwise");
    } else {
      direction = 1; // Change to clockwise
      Serial.println("Changing direction to clockwise");
    }


  }
  digitalWrite(DIR_PIN, direction);
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
}
