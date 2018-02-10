#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 3
#define FASTLED_ESP8266_DMA
#include <FastLED.h>
#include <ArduinoJson.h>
#include "eepromhandler.h"

class WS2812BStrip {
    int pin = -1;     
    String commandIn;
    EEPROMHandler* eepromhandler;
    
    // Work variables
    CRGB* leds = NULL; // Initial value    
    int lednum = 0; 
    String color = "FFFFFF";
    String prevColor = "";
    int brightness = 255;
    int prevBrightness = 0;
    
  public:    
    WS2812BStrip(EEPROMHandler& eepromhandler){       
       this->eepromhandler = &eepromhandler;       
    }

    void setup(int pin, String commandIn) {
      this->pin = pin;
      this->commandIn = commandIn;
      Serial.print("Led strip setup pin: ");      
      Serial.println(this->pin);

      // Fastled should configure with constant
      // Because of this i need to preconfigure the declarations
      // Currently just two pins are allowed, because of this
      if (pin > -1 && (pin == 4 || pin == 5)){
       lednum = eepromhandler->getValueAsInt("lednum", false);
       leds = new CRGB[lednum];

       if (pin == 5) {
          FastLED.addLeds<WS2812B, 5, GRB>(leds, lednum).setCorrection( TypicalLEDStrip );
       }

       if (pin == 4) {
          FastLED.addLeds<WS2812B, 4, GRB>(leds, lednum).setCorrection( TypicalLEDStrip );          
       }
       FastLED.setBrightness(brightness);
       
       Serial.println("Led strip setup ready");     
      }
    }

    void loop() {
      if (pin > -1){
        if (color != prevColor || brightness != prevBrightness) {
          FillLEDsColors();
          FastLED.setBrightness(brightness);
          FastLED.show();
          FastLED.delay(10);
          prevColor = color;
        }
      }
    }

    void trigger(char* topic, String payload){
      if (pin > -1){
        String topic_str = String(topic);
        // Remove the wildcard character
        this->commandIn.replace("/#", "");
        if (topic_str == (this->commandIn + "/color")) {
          payload.replace("#", "");
          this->color = payload;          
          Serial.print("Set color to #");          
          Serial.print(color);
        }

        if (topic_str == (this->commandIn + "/brightness")) {          
          this->brightness = atoi( payload.c_str() );
          Serial.print("Set brightness to ");
          Serial.println(brightness);
        }
      }
    }
    private:

    void FillLEDsColors()
    {
        uint8_t brightness = 255;
        
        for( int i = 0; i < lednum; i++) {            
            leds[i] = strtoul(color.c_str(), (char **)NULL, 16);
        }
    }
    /*
     private methods comes here
    void privatefunc(){
    
    }
    */
};
