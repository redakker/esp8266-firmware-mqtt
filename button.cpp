#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "eepromhandler.h"

class Button {
    int pin = -1;
    String type; /* stateful, stateless */
    PubSubClient* clnt;    
    String commandOut;
    String device;
    EEPROMHandler* eepromhandler;

    // Work variables
    String lastvalue;
    
  public:
    
    Button(PubSubClient& client, EEPROMHandler& eepromhandler){
       clnt = &client;
       this->eepromhandler = &eepromhandler;       
              
    }

    /*
     * There is two type button:
     * - stateful: event comes in both states. We must know the actual state
     * - stateless: event comes just when the button pushed. Send once when the button pushed and don't store the state
    */
    void setup(int pin, String type, String commandOut) {
      this->pin = pin;
      this->type = type;
      this->commandOut = commandOut;

      Serial.print("Button setup pin: ");
      Serial.print(this->pin);
      Serial.print(" type:");
      Serial.println(this->type);
      
      if (pin > -1){
        pinMode(pin, INPUT);      
        lastvalue = "";
        Serial.println("Button setup ready");
        this->device = eepromhandler->getValueAsString("device", true);           
      }
    }

    void loop() {
      if (pin > -1){   
        boolean buttonState = digitalRead(pin);
        if (type == "stateful"){          
          if (buttonState == HIGH && lastvalue != "HIGH") {
            Serial.println("Button switched on");
            lastvalue = "HIGH";                        
            String message = "{device: \"" + this-> device + "\",type:\"buttonstateful\",value:1}";
            clnt->publish(commandOut.c_str(), (char*) message.c_str());
            Serial.println("Send button state 1");
            delay(10);
          }
        
          if (buttonState == LOW && lastvalue != "LOW") {
            Serial.println("Button switched off");
            lastvalue = "LOW";            
            String message = "{device: \"" + this-> device + "\",type:\"buttonstateful\",value:0}";
            clnt->publish(commandOut.c_str(), (char*) message.c_str());
            Serial.println("Send button state 0");
            delay(10);
          }
        }
  
        // In this case send a message just when the button was pushed
        if (type == "stateless"){
          if (buttonState == LOW) {
              while (!digitalRead(this->pin)) {}
              Serial.println("Button pushed");
              String message = "{device: \"" + this-> device + "\",type:\"buttonstateless\",value:1}";
              clnt->publish(commandOut.c_str(), (char*) message.c_str());              
              Serial.println("Send state 1");          
              delay(10);
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
