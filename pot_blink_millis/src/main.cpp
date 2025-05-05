/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

 #include <Arduino.h>

 // Set LED_BUILTIN if it is not defined by Arduino framework
 #ifndef LED_BUILTIN
     #define LED_BUILTIN 2
 #endif
 
 #define LED_PIN 18 // Define the LED_PIN pin number
 #define POT_PIN 34 // Define the POT_PIN pin number
 
 void setup()
 {
    pinMode(LED_PIN, OUTPUT);
    pinMode(POT_PIN, INPUT);

    Serial.begin(115200);                                    // Initialize Serial coms

    delay(500);                                              // Wait for 500ms to allow the serial monitor to open
    Serial.println("Esp32 starter!");                        // Print a message to the serial monitor
    Serial.println("LED_PIN: " + String(LED_PIN));           // Print the LED_PIN pin number
 }
 
 unsigned long previusMilis = 0;                                 // Variable to store the current time in milliseconds
 bool ledState = LOW;                                          // Variable to store the LED state

 void loop()
 {
    int potValue = analogRead(POT_PIN);                     // Read the value from the potentiometer
    // Serial.println("Pot read: " + String(potValue));        // Print the value to the serial monitor

    int blinkDelay = map(potValue, 0, 4095, 30, 512);       // Map the potentiometer value to a range of 32 to 512
    Serial.println("Blink delay: " + String(blinkDelay));   // Print the mapped value to the serial monitor

    unsigned long currentMillis = millis();                  // Get the current time in milliseconds 
    
    Serial.println("currentMillis - previusMilis >= blinkDelay: "
                   + String(currentMillis) + " - " + String(previusMilis) + " >= "  + String(blinkDelay) + " : " +  String(currentMillis - previusMilis >= blinkDelay) ); // Print the current time to the serial monitor
    
    if (currentMillis - previusMilis >= blinkDelay) 
    {  
        ledState = !ledState;                               // Toggle the LED state

        digitalWrite(LED_PIN, ledState);                    // Set the LED state to the current state
        Serial.println("LED State: " + String(ledState));   // Print the LED state to the serial monitor  
        previusMilis = currentMillis;                       // Update the `previousMillis` to the current time  
    }
}
                             
 