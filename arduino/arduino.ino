
/*
 * myTimerEvent() updated
 * we remove delay and replaced by changing 
 * pin number
 */
#include "Ticker.h"
#include <Arduino_JSON.h>
#include <ArduinoJson.h>

//********************* Variables declaration **********************

JSONVar switch_status; // Last status of switch
JSONVar last_status; // Last status of switch


unsigned long lastDebounceTime[7]; // the last time the output pin was toggled - 6: the feedback numbers
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers - 25 = 50, 30 = 60...

int pin_update = 2;



void sendBlynk(int pin, int value){
  StaticJsonDocument<200> doc;
  doc["pin"] = pin;
  doc["value"] = value;
  serializeJson(doc, Serial);
}



void value99()
{ 
   int x = 0;
   for (int i=2; i<9; i++){
      if (digitalRead(i) == 1){
        x++;
      }
   }
   sendBlynk(99, x);
}


void myTimerEvent()
{
   sendBlynk(pin_update, digitalRead(pin_update));
   value99();
   pin_update++;
   if (pin_update==9)pin_update=2;
}




Ticker timer(myTimerEvent, 500);    


void setup(){
 Serial.begin(9600);
 delay(2000);
 
 for (int i=2; i<9; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW); 
 }


 for (int i=9; i<16; i++){
    pinMode(i, INPUT);
    switch_status[String(i)] = 0 ;
    last_status[String(i)] = 0 ;
 }


 timer.start();
}

void loop()
{
    timer.update();
    
    while (Serial.available()) {
      StaticJsonDocument<300> doc;
      DeserializationError err = deserializeJson(doc, Serial);
      if (err == DeserializationError::Ok) 
      {
        Serial.print("pin: ");
        Serial.println(doc["pin"].as<int>());
        Serial.print("value: ");
        Serial.println(doc["value"].as<int>());
        int pin = doc["pin"].as<int>();
        int value = doc["value"].as<int>();
        if (pin > 1 && pin < 9){
          digitalWrite(pin, !digitalRead(value));
          value99();
        }
      
      } 
      else 
      {
        // Print error to the "debug" serial port
        Serial.print("deserializeJson() returned ");
        Serial.println(err.c_str());
    
        // Flush all bytes in the "link" serial port buffer
        while (Serial.available() > 0)
          Serial.read();
      }
    }


    for (int i =9; i<16 ; i++){
      int reading = digitalRead(i);
      if (reading != (int)last_status[String(i)]){
         lastDebounceTime[i-9] = millis(); 
         last_status[String(i)] = reading;
      }
      if ((millis() - lastDebounceTime[i-9]) > debounceDelay) 
      {
         if (reading != (int)switch_status[String(i)]){
             switch_status[String(i)] = reading;
             if (reading == HIGH){
               digitalWrite(i-7, !digitalRead(i-7));
               sendBlynk(i-7, digitalRead(i-7));
               value99();
             }
         }
      }
      
    }
  
  
}
