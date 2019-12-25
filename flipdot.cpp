#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>  
#include <LinkedList.h>

class FlipDot {

    const int DISPLAY_MAX_LINES = 7;
    const int DISPLAY_MAX_COLUMNS = 28;
    int pin = -1;
    LinkedList<String> lines = LinkedList<String>();
    String commandIn = "";
    bool textchanged = false;
    
  public:    
    FlipDot(){
      
    }
    
    void setup(int PIN, String commandIn) {

      this->pin = PIN;
     
      this->commandIn = commandIn;
    }

    void loop() {
      if (this->pin > -1 ){
        // Should not draw the same text every loop
        if (textchanged){          
          for (int i = 0; i < this->lines.size(); i++){
              String line = this->lines.get(i);              
          }
          // write the buffer to the display
          
        }
      }
    }

    void displayText(String text){
      if (text.length() >= DISPLAY_MAX_COLUMNS) {
        for (int i = 0; i <= text.length() / DISPLAY_MAX_COLUMNS; i++) {
          addLine(text.substring(i * DISPLAY_MAX_COLUMNS, (i + 1) * DISPLAY_MAX_COLUMNS));
        }
      } else {
        addLine(text);
      }
    }

    void trigger(char* topic, String payload) {
      if (this->pin > -1){
        String topic_str = String(topic);
        // Remove the wildcard character
        this->commandIn.replace("/#", "");
        if (topic_str == (this->commandIn + "/flipdot")) {          
          Serial.print("Flipdot data received");          
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
      if (lines.size() >= DISPLAY_MAX_LINES){
        lines.remove(0);
      }
      this->lines.add(text);
      textchanged = true;
    }
};
