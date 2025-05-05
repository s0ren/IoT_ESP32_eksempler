#include <Arduino.h>

#define BUTTON_PIN 35  // GPIO21 til knap

int lastState = LOW;  // starter i LOW tilstand, pga fysisk pull down modstand

void setup() {
  Serial.begin(115200);  // hurtigere baudrate giver mere præcis log
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  int currentState = digitalRead(BUTTON_PIN);

  // if (currentState != lastState) {
   
    Serial.println(">btnState: " + String(currentState));
    // lastState = currentState;
  // }

  // delay(1);  // 1 ms sampling – hurtigt nok til at fange bouncing
}
