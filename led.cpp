#include <PubSubClient.h>
#include <ArduinoJson.h>

class Led {
    int pin = -1;   
    
  public:    
    Led(){
      
    }
    
    void setup(int pin) {
      this->pin = pin;

      Serial.print("Led setup pin: ");      
      Serial.println(this->pin);
      
      if (pin > -1){
       pinMode(pin, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
       operation(100, 5);
       Serial.println("LED setup ready");
      }
    }

    void loop() {
      if (pin > -1){
            
      }
    }

    // Flashing LED which means message received. Switch off after, not to disturb the enviroment.
    // Delay parameter to see the operation code
    void operation(int d, int count) {
      if (this->pin > -1){
        for (int i = 0; i < count; i++) {        
          digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
          delay(d);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(d);        
        }
        Serial.println("Led operation done");
        Serial.print(" ");
        Serial.print(d);
        Serial.print(",");
        Serial.println(count);
      }
    }
    
    private:
    /*
     private methods comes here
    void privatefunc(){
    
    }
    */
};
