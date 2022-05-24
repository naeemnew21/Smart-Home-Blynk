
#define RXD2 16
#define TXD2 17

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLAF8XX1Cr"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "zDQhJxNEgvs50qrKGcApnuyEWV_-w7Rt"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ANaeem";
char pass[] = "11166+##=11";


BLYNK_WRITE_DEFAULT()
{
  int pin = request.pin;      // Which exactly pin is handled?
  int value = param.asInt();  // Use param as usual.
  
  StaticJsonDocument<200> doc;
  doc["pin"] = pin;
  doc["value"] = value;
  serializeJson(doc, Serial2);
  digitalWrite(2, HIGH);
  delay(250);
  digitalWrite(2, LOW);
}


void setup()
{
  // Debug console
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();

  while (Serial2.available()) {
      StaticJsonDocument<300> doc;
      DeserializationError err = deserializeJson(doc, Serial2);
      if (err == DeserializationError::Ok) 
      {
        Serial.print("pin: ");
        Serial.println(doc["pin"].as<int>());
        Serial.print("value: ");
        Serial.println(doc["value"].as<int>());
        int pin = doc["pin"].as<int>();
        int value = doc["value"].as<int>();
        Blynk.virtualWrite(pin, value);
      
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
}
