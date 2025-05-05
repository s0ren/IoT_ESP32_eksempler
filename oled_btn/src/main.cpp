#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define LED_PIN 18 // Define the LED_PIN pin number
#define BTN_PIN 35

void display_print(String text); // Function prototype for display_print

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT); // Set the LED_PIN pin as output
  pinMode(BTN_PIN, INPUT_PULLDOWN); // Set the BTN_PIN pin as input

  Serial.begin(115200); // Initialize Serial communication at 115200 baud rate
  delay(500); // Wait for 500ms to allow the serial monitor to open
  Serial.println("Esp32 starter!"); // Print a message to the serial monitor

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(500);

  display_print(String("Hej mormor!"));
}

void display_print(String text)
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println(text);
  display.display();
  delay(100);
}

bool ledState = LOW; // Variable to store the LED state
bool lastBtnState = HIGH; // Variable to store the last button state

void loop() {
  
  int btnState = digitalRead(BTN_PIN);                    // Read the state of the button
  // Serial.println(">Button_state:" + String(btnState));    // Print the button state to the serial monitor

  digitalWrite(LED_PIN, ledState);                       // Set the LED state to the current state
  // Serial.println("LED State: " + String(ledState));      // Print the LED state to the serial monitor  

  // Serial.println("btnState: " + String(btnState) + " lastBtnState: " + String(lastBtnState)); // Print the button state to the serial monitor

  if (btnState == HIGH && lastBtnState != btnState) {                                 // If the button is pressed
    ledState = !ledState;                                                             // Toggle the LED state
    display_print(ledState ? "LED ON" : "LED OFF");                                   // Print the LED state to the display                       
  }

  lastBtnState = btnState;                             // Update the last button state
  // delay(100);
}
