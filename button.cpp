#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "eepromhandler.h"

class Button {
    int pin = -1;
    String type; /* stateful, stateless */
    PubSubClient* clnt;    
    String commandOut;
    EEPROMHandler* eepromhandler;

    // Work variables
    String lastvalue;
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    char jsonChar[500];
    
    
  public:
    
    Button(PubSubClient& client, EEPROMHandler& eepromhandler, String commandOut){
       clnt = &client;
       this->eepromhandler = &eepromhandler;       
       this->commandOut = commandOut;       
    }

    /*
     * There is two type button:
     * - stateful: event comes in both states. We must know the actual state
     * - stateless: event comes just when the button pushed. Send once when the button pushed and don't store the state
    */
    void setup(int pin, String type) {
      this->pin = pin;
      this->type = type;

      Serial.print("Button setup pin: ");
      Serial.print(this->pin);
      Serial.print(" type:");
      Serial.println(this->type);
      
      if (pin > -1){
        pinMode(pin, INPUT);      
        lastvalue = "";
        Serial.println("Button setup ready");
      }
    }

    void loop() {
      if (pin > -1){   
        boolean buttonState = digitalRead(pin);
        root["device"] = eepromhandler->getValueAsString("device", true);
        root["type"] = "button";
        root["pin"] = pin;
        
        if (type == "stateful"){
          root["buttontype"] = "stateful";    
          if (buttonState == HIGH && lastvalue != "HIGH") {
            Serial.println("Button switched on");
            lastvalue = "HIGH";            
            root["value"] = "1";
            root.printTo((char*)jsonChar, root.measureLength() + 1);
            clnt->publish(commandOut.c_str(), jsonChar);
            Serial.println("Send button state 0");
          }
        
          if (buttonState == LOW && lastvalue != "LOW") {
            Serial.println("Button switched off");
            lastvalue = "LOW";            
            root["value"] = "0";
            root.printTo((char*)jsonChar, root.measureLength() + 1);
            clnt->publish(commandOut.c_str(), jsonChar);
            Serial.println("Send button state 0");
          }
        }
  
        // In this case send a message just when the button was pushed
        if (type == "stateless"){
          if (buttonState == LOW && lastvalue != "LOW") {
              lastvalue = "LOW";
              Serial.println("Button pushed");
              root["buttontype"] = "stateless";
              root["value"] = "1";
              root.printTo((char*)jsonChar, root.measureLength() + 1);            
              clnt->publish(commandOut.c_str(), jsonChar, true);
              Serial.println("Send state 1");          
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


