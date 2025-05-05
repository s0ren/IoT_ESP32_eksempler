/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-vs-code-platformio-littlefs/
*********/

#include <Arduino.h>
#include "LittleFS.h"
 
void setup() {
  Serial.begin(9600);

  delay(1000);
  Serial.println("LittleFS Example");
  
  if(!LittleFS.begin(true)){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  
  File file = LittleFS.open("/text.txt");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  Serial.print("File Content: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
  Serial.println();
}
 
void loop() {

}