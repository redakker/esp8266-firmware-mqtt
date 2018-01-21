#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "eepromhandler.h"

class Motion {
    int pin = -1;   
    String commandOut;
    PubSubClient* clnt;    
    EEPROMHandler* eepromhandler;
    
    // Work variables
    bool pirState = HIGH;
    int val = LOW;
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    char jsonChar[500];
    
  public:    
    Motion(PubSubClient& client, EEPROMHandler& eepromhandler){
      clnt = &client;
      this->eepromhandler = &eepromhandler;
    }
    
    void setup(int pin, String commandOut) {
      this->pin = pin;

      this->commandOut = commandOut;

      if (pin > -1){                
        Serial.println("Motion setup pin ready");
        root["device"] = eepromhandler->getValueAsString("device", false);
        root["type"] = "motion";
      }

      Serial.print("Motion setup pin: ");      
      Serial.println(this->pin);
      
      if (pin > -1){
       pinMode(pin, INPUT);
       Serial.println("Motion setup ready");
      }
    }

    void loop() {
      if (pin > -1){
          val = digitalRead(pin);  // read input value
          if (val == HIGH) {            // check if the input is HIGH            
            if (pirState == LOW) {
              // we have just turned on
              Serial.println("Motion detected!");
              
              root["motion"] = "moving";
              root.printTo((char*)jsonChar, root.measureLength() + 1);
              clnt->publish(commandOut.c_str(), jsonChar, true);
              
              // We only want to print on the output change, not state
              pirState = HIGH;
            }
          } else {            
            if (pirState == HIGH){
              // we have just turned of
              Serial.println("Motion ended!");
              
              root["motion"] = "not_moving";
              root.printTo((char*)jsonChar, root.measureLength() + 1);
              clnt->publish(commandOut.c_str(), jsonChar, true);
              
              // We only want to print on the output change, not state
              pirState = LOW;
            }
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
