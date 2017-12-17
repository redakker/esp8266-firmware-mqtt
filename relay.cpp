#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "eepromhandler.h"

class Relay {
    int pin = -1;     
    String commandIn;
    EEPROMHandler* eepromhandler;
    
    // Work variables
    String lastvalue;
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    char jsonChar[500];
    
    
  public:    
    Relay(EEPROMHandler& eepromhandler){       
       this->eepromhandler = &eepromhandler;       
    }

    void setup(int pin, String commandIn) {
      this->pin = pin;
      this->commandIn = commandIn;
      this->commandIn.replace("#", "");
      Serial.print("Relay setup pin: ");      
      Serial.println(this->pin);
      
      if (pin > -1){
       pinMode(pin, OUTPUT);     // Initialize the RELAY pin as an output
       digitalWrite(pin, LOW);   // Default OFF
       Serial.println("Relay setup ready");     
      }
    }

    void loop() {
      if (pin > -1){
            
      }
    }

    void trigger(char* topic, String payload){
      if (pin > -1){
        String topic_str = String(topic);
        if (topic_str == commandIn) {
          if (payload == "1") {
            digitalWrite(pin, HIGH);
            //operation(100, 4);
          } else {
            digitalWrite(pin, LOW);
            //operation(500, 4);
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
