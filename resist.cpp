#include <PubSubClient.h>
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
    int interval = 0 ;
    
  public:    
    Resist(PubSubClient& client, EEPROMHandler& eepromhandler){       
       clnt = &client;
       this->eepromhandler = &eepromhandler;       
    }

    void setup(String name, int pin, String type, String commandOut) {
      this->pin = pin;
      this->type = type;
      this->name = name;
      this->commandOut = commandOut;

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
        this->interval = eepromhandler->getValueAsInt("interval", false);
      }
    }

    void loop() {
      if (pin > -1){       
        if (millis() - lastSend > interval){
          lastSend = millis();          

          String message = "{name: \"" + name + "\",analogdigital:\"" + type + "\", value:\"" + sensorValue + "\"}";
          clnt->publish(commandOut.c_str(), (char*)message.c_str(), true);

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
