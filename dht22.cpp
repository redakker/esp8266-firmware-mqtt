#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>;
#include "eepromhandler.h"

class DHT_22 {
    int pin = -1;
    String commandOut;
    String device;
    PubSubClient* clnt;
    DHT* dht = NULL;
    EEPROMHandler* eepromhandler;

    // Work variables
    float humidity;
    float temperature;
    unsigned long lastSend;    
    char jsonChar[500];
    int interval = 0;
        
    
  public:    
    DHT_22(PubSubClient& client, EEPROMHandler& eepromhandler){       
       clnt = &client;
       this->eepromhandler = &eepromhandler;
             
    }

    void setup(int pin, String commandOut) {
      this->pin = pin;
      this->commandOut = commandOut;

      Serial.print("DHT22 setup pin: ");
      Serial.println(this->pin);      
      
      if (pin > -1){
        dht = new DHT(pin, DHT22);
        dht->begin();
        lastSend = 0;
        Serial.println("DHT22 setup ready");
        this->device = eepromhandler->getValueAsString("device", false);
        
        this->interval = eepromhandler->getValueAsInt("interval", false);        
      }
    }

    void loop() {
      if (pin > -1){
        //Serial.println("loop DHT22");
        if (millis() - lastSend > interval){
          lastSend = millis();                   
          humidity = dht->readHumidity();
          temperature = dht->readTemperature();          

          String message = "{device: \"" + this->device + "\", type: \"dht22\", temperature: \"" + temperature + "\", humidity: \"" + humidity + "\"}";
          clnt->publish(commandOut.c_str(), (char*)message.c_str(), true);

          Serial.println("Send DHT22 data: ");
          Serial.println(humidity);          
          Serial.println(temperature);
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
