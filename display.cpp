#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>  
#include "SSD1306.h" //https://github.com/squix78/esp8266-oled-ssd1306
#include <LinkedList.h>

class Display {

    const int DISPLAY_MAX_LINES = 5;
    const int DISPLAY_MAX_COLUMNS = 20;
    int pin_sda = -1;
    int pin_sdc = -1;
    SSD1306* display = NULL;
    LinkedList<String> lines = LinkedList<String>();
    String commandIn = "";
    
  public:    
    Display(){
      
    }
    
    void setup(int PIN_SDA, int PIN_SDC, String commandIn) {

      this->pin_sda = PIN_SDA;
      this->pin_sdc = PIN_SDC;
      
      display = new SSD1306(0x3c,PIN_SDA,PIN_SDC);
      // Initialising the UI will init the display too.
      display->init();    
      display->flipScreenVertically();
      display->setFont(ArialMT_Plain_10);
      this->commandIn = commandIn;
    }

    void loop() {
      if (this->pin_sda > -1 && this->pin_sdc > -1 ){
        display->clear();
        
        display->setTextAlignment(TEXT_ALIGN_LEFT);
        for (int i = 0; i < this->lines.size(); i++){
            String line = this->lines.get(i);
            display->setFont(ArialMT_Plain_10);
            display->drawString(0, i*10, line);  
        }
        // write the buffer to the display
        
        display->display();              
      }
    }

    void displayText(String text){
      if (text.length() > DISPLAY_MAX_COLUMNS) {
        for (int i = 0; i <= text.length() / DISPLAY_MAX_COLUMNS; i++) {
          addLine(text.substring(i * DISPLAY_MAX_COLUMNS, (i + 1) * DISPLAY_MAX_COLUMNS));
        }
      } else {
        addLine(text);
      }
    }

    void trigger(char* topic, String payload) {
      if (this->pin_sda > -1 && this->pin_sdc > -1 ){
        String topic_str = String(topic);
        // Remove the wildcard character
        this->commandIn.replace("/#", "");
        if (topic_str == (this->commandIn + "/text")) {
          this->displayText(payload);
          Serial.print(payload);
          Serial.println(" - added to lines");
        }
      }
    }
 
    private:
    /*
     private methods comes here
    void privatefunc(){
    
    }
    */

    void addLine(String text) {
      if (lines.size() > DISPLAY_MAX_LINES){
        lines.remove(0);
      }
      this->lines.add(text);
    }
};
