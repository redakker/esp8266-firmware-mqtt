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
    const int MAX_RESULT_NUMBER = 4;
    const char DELIMITER = ',';

  public:
    WS2812BStrip(EEPROMHandler& eepromhandler) {
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
      if (pin > -1 && (pin == 4 || pin == 5)) {
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
      if (pin > -1) {
        if (color != prevColor || brightness != prevBrightness) {
          FillLEDsColors();
          FastLED.setBrightness(brightness);
          FastLED.show();
          FastLED.delay(10);
          prevColor = color;
        }
      }
    }

    void trigger(char* topic, String payload) {
      if (pin > -1) {
        String topic_str = String(topic);
        // Remove the wildcard character
        this->commandIn.replace("/#", "");
        if (topic_str == (this->commandIn + "/color")) {
          payload.replace("#", "");

          String givenColor = "FFFFFF";

          // Decide if the given color is HEX or RGB
          // If the length is 6 char then assume this is a HEX color string
          // otherwise that is RGB
          if (payload.length() == 6) {
            givenColor = payload;
          } else {
            // RGB, clean and split the string
            // Format must be rrr,ggg,bbb
            payload.replace("[", "");
            payload.replace("]", "");
            payload = payload + DELIMITER;

            String result[MAX_RESULT_NUMBER];                  
            splitString(payload, DELIMITER, result);

            String r = result[0];
            String g = result[1];
            String b = result[2];

            Serial.print(payload);
            Serial.print(" Splitted to: ");
            Serial.print(r);
            Serial.print(",");
            Serial.print(g);
            Serial.print(",");
            Serial.println(b);

            givenColor = decimalToHexadecimal(r);
            givenColor += decimalToHexadecimal(g);
            givenColor += decimalToHexadecimal(b);

          }

          this->color = givenColor;
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

      for ( int i = 0; i < lednum; i++) {
        leds[i] = strtoul(color.c_str(), (char **)NULL, 16);
      }
    }

    String decimalToHexadecimal(String colorCode) {
      
      int dec = 0;
      
      dec = colorCode.toInt();
           
      if (dec < 1) return "00";

      int hex = dec;
      String hexStr = "";
      hexStr = String (dec, HEX);
      return hexStr;
    }

    void splitString(String text, char splitChar, String result[]) {
      int splitCount = countSplitCharacters(text, splitChar);
      char buff[text.length() + 1];
      text.toCharArray(buff, text.length() + 1);
      int counter = 0;
      int from = 0;

      for (int i = 0; i < sizeof(buff) - 1; i++) {
        if (splitCount < MAX_RESULT_NUMBER) {
          if (buff[i] == splitChar) {
            result[counter] = text.substring(from, i);
            result[counter].trim();            
            from = i + 1;
            counter++;
          }
        } else {
          Serial.print("Warning! The max result number is smaller than the given data. Need to modify the source code. Max result number is ");
          Serial.println(MAX_RESULT_NUMBER);
          break;
        }
      }
    }

    int countSplitCharacters(String text, char splitChar) {
      int returnValue = 0;
      char buff[text.length() + 1];
      text.toCharArray(buff, text.length() + 1);

      for (int i = 0; i < sizeof(buff) - 1; i++) {
        if (buff[i] == splitChar) {
          returnValue++;
        }
      }
      return returnValue;
    }
    /*
      private methods comes here
      void privatefunc(){

      }
    */
};
