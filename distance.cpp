#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "eepromhandler.h"
/*
   Code source from: https://www.tautvidas.com/blog/2012/08/distance-sensing-with-ultrasonic-sensor-and-arduino/ 
 
   This object reads a HC-SR04 ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse 
   to return.  The length of the returning pulse is proportional to 
   the distance of the object from the sensor.
     
   The circuit:
  * VCC connection of the sensor attached to +5V
  * GND connection of the sensor attached to ground
  * TRIG connection of the sensor attached to web configured digital pin 
  * ECHO connection of the sensor attached to web configured digital pin
*/

class Distance {
    int triggerPin = -1;
    int echoPin = -1;
    String commandOut;
    PubSubClient* clnt;
    EEPROMHandler* eepromhandler;

    // Work variables
    float distance;    
    unsigned long lastSend;    
    char jsonChar[500];
    int interval = 0;
    
  public:    
    Distance(PubSubClient& client, EEPROMHandler& eepromhandler){
      clnt = &client;
      this->eepromhandler = &eepromhandler;      
    }
    
    void setup(int triggerPin, int echoPin, String commandOut) {
      this->triggerPin = triggerPin;
      this->echoPin = echoPin;
      this->commandOut = commandOut;
     
      Serial.print("Distance setup pin:\nTrigger: ");      
      Serial.println(this->triggerPin);
      Serial.print("Echo: ");      
      Serial.println(this->echoPin);

      if (triggerPin > -1 && echoPin > -1){        
        this->interval = eepromhandler->getValueAsInt("interval", false);
        Serial.println("Distance setup ready");        
      }
      
    }
    void loop() {

      if (triggerPin > -1 && echoPin > -1){
        if (millis() - lastSend > interval){
            lastSend = millis();
            // establish variables for duration of the ping, 
            // and the distance result in inches and centimeters:
            long duration, inches, cm;
          
            // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
            // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
            pinMode(triggerPin, OUTPUT);
            digitalWrite(triggerPin, LOW);
            delayMicroseconds(2);
            digitalWrite(triggerPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(triggerPin, LOW);
          
            // Read the signal from the sensor: a HIGH pulse whose
            // duration is the time (in microseconds) from the sending
            // of the ping to the reception of its echo off of an object.
            pinMode(echoPin, INPUT);
            duration = pulseIn(echoPin, HIGH);
          
            // convert the time into a distance
            inches = microsecondsToInches(duration);
            cm = microsecondsToCentimeters(duration);
  
            String message = "{type: \"dht22\", cm: \"" + String(cm) + "\", inch: \"" + String(inches) + "\"}";
            clnt->publish(commandOut.c_str(), (char*) message.c_str(), true);
  
            Serial.print("Send DHT22 data: ");
            Serial.print(inches);
            Serial.print(" in, ");
            Serial.print(cm);
            Serial.print(" cm");
            Serial.println();         
            
            delay(100);
        }
      }
    }
    private:
    /*
     private methods comes here
    void privatefunc(){
    
    }
    */

    long microsecondsToInches(long microseconds) {
      // According to Parallax's datasheet for the PING))), there are
      // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
      // second).  This gives the distance travelled by the ping, outbound
      // and return, so we divide by 2 to get the distance of the obstacle.
      // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
      return microseconds / 74 / 2;
    }
    
    long microsecondsToCentimeters(long microseconds) {
      // The speed of sound is 340 m/s or 29 microseconds per centimeter.
      // The ping travels out and back, so to find the distance of the
      // object we take half of the distance travelled.
      return microseconds / 29 / 2;
    }
};
