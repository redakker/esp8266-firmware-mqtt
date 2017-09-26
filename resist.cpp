#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>;
#include "eepromhandler.h"

class Resist {
    int pin = -1;
    String commandOut;
    String type;
    String name;
    PubSubClient* clnt;
    EEPROMHandler* eepromhandler;
    
    // Work variables
    int sensorValue = 0;
    unsigned long lastSend;
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    char jsonChar[500];        
    
  public:    
    Resist(PubSubClient& client, EEPROMHandler& eepromhandler, String commandOut){       
       clnt = &client;
       this->eepromhandler = &eepromhandler;
       this->commandOut = commandOut;       
    }

    void setup(String name, int pin, String type) {
      this->pin = pin;
      this->type = type;
      this->name = name;

      Serial.print("Resist setup pin: ");
      Serial.print(this->pin);
      Serial.print(" type:");
      Serial.print(this->type);
      Serial.print(" name:");
      Serial.println(this->name);
      
      if (pin > -1){
        if (type == "digital"){
          pinMode(pin, INPUT); 
        }
      }
    }

    void loop() {
      if (pin > -1){       
        if (millis() - lastSend > eepromhandler->getValueAsInt("interval", true)){
          lastSend = millis();          
          root["device"] = eepromhandler->getValueAsString("device", false);
          root["type"] = name;
          root["analogdigital"] = type;

          if (type == "analog"){
            sensorValue = analogRead(pin);
          }

          if (type == "digital"){
            sensorValue = digitalRead(pin);
          }
          
          root[name] = sensorValue;          

          root.printTo((char*)jsonChar, root.measureLength() + 1);
          clnt->publish(commandOut.c_str(), jsonChar);

          Serial.print("Moisture sensor data: ");
          Serial.println(sensorValue);          
        }
      }
    }
    
    private:
    /*
     private methods comes here
    void privatefunc(){
    
    }
    */
};
