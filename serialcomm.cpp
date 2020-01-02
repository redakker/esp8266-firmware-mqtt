#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// This class makes a SoftwerSerial connection on a given two pins.
// It translates and forward MQTT messages to another (offline) machine on serial connection
// MQTT topic must be "serial" for example: /home/this/device/path/in/serial
// MQTT body will be sent out to the serial
class SerialComm {
    int txPin = -1;
    int rxPin = -1;    
    SoftwareSerial* serial;
    
    // Work variables
    String commandIn = "";
    
  public:    
    SerialComm(){              
    }

    void setup(int txPin, int rxPin, String commandIn) {
      this->commandIn = commandIn;
      Serial.println("Soft serial port is set up.");
      Serial.print("tx pin: ");
      Serial.println(txPin);
      Serial.print("rx pin: ");
      Serial.println(rxPin);

      this->txPin = txPin;
      this->rxPin = rxPin;
      
      if (this->txPin > -1 && this->rxPin > -1 ){
        this->serial = new SoftwareSerial(this->rxPin, this->txPin);     
        this->serial->begin(115200);
      }
    }

    void loop() {
            
    }

    void trigger(char* topic, String payload) {
      if (this->txPin > -1 && this->rxPin > -1 ){
        String topic_str = String(topic);
        // Remove the wildcard character
        this->commandIn.replace("/#", "");
        if (topic_str == (this->commandIn + "/serial")) {
          Serial.println("Message sent out in a configured softserial port:");
          Serial.println(payload);
          this->serial->println(payload);          
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
