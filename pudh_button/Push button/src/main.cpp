#include <Arduino.h>

#define LED_PIN 18 // Define the LED_PIN pin number
#define BTN_PIN 35

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT); // Set the LED_PIN pin as output
  pinMode(BTN_PIN, INPUT_PULLDOWN); // Set the BTN_PIN pin as input

  Serial.begin(115200); // Initialize Serial communication at 115200 baud rate
  delay(500); // Wait for 500ms to allow the serial monitor to open
  Serial.println("Esp32 starter!"); // Print a message to the serial monitor
}

bool ledState = LOW; // Variable to store the LED state
bool lastBtnState = HIGH; // Variable to store the last button state

void loop() {
  
  int btnState = digitalRead(BTN_PIN);                    // Read the state of the button
  Serial.println(">Button_state:" + String(btnState));    // Print the button state to the serial monitor

  digitalWrite(LED_PIN, ledState);                       // Set the LED state to the current state
  // Serial.println("LED State: " + String(ledState));      // Print the LED state to the serial monitor  

  // Serial.println("btnState: " + String(btnState) + " lastBtnState: " + String(lastBtnState)); // Print the button state to the serial monitor

  if (btnState == HIGH && lastBtnState != btnState) {                                 // If the button is pressed
    ledState = !ledState;                                  // Toggle the LED state
  }

  lastBtnState = btnState;                             // Update the last button state
  // delay(100);
}
