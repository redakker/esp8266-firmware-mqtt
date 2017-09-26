#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>;
#include "eepromhandler.h"

class DHT_22 {
    int pin = -1;
    String commandOut;
    PubSubClient* clnt;
    DHT* dht = NULL;
    EEPROMHandler* eepromhandler;

    // Work variables
    float humidity;
    float temperature;
    unsigned long lastSend;
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    char jsonChar[500];
        
    
  public:    
    DHT_22(PubSubClient& client, EEPROMHandler& eepromhandler, String commandOut){       
       clnt = &client;
       this->eepromhandler = &eepromhandler;
       this->commandOut = commandOut;       
    }

    void setup(int pin) {
      this->pin = pin;

      Serial.print("DHT22 setup pin: ");
      Serial.println(this->pin);      
      
      if (pin > -1){
        dht = new DHT(pin, DHT22);
        dht->begin();
        lastSend = 0;
        Serial.println("DHT22 setup ready");
      }
    }

    void loop() {
      if (pin > -1){
        //Serial.println("loop DHT22");
        if (millis() - lastSend > eepromhandler->getValueAsInt("interval", true)){
          lastSend = millis();
          root["device"] = eepromhandler->getValueAsString("device", false);
          root["type"] = "dht22";          
          humidity = dht->readHumidity();
          temperature = dht->readTemperature();
          root["temperature"] = temperature;
          root["humidity"] = humidity;

          root.printTo((char*)jsonChar, root.measureLength() + 1);
          clnt->publish(commandOut.c_str(), jsonChar);

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
