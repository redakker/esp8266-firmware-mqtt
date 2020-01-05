#include <ArduinoJson.h>

// http://playground.arduino.cc/Code/EepromUtil
// Before compile should add to the compiler's libraries folder 
#include "EepromUtil.h"
#include "eepromhandler.h"
// Make more easy to use EEPROMUtil read this:
// http://mario.mtechcreations.com/programing/write-string-to-arduino-eeprom/

// constructor
EEPROMHandler::EEPROMHandler() {
       eepromutil = new EepromUtil();
       EEPROM.begin(512);       
}

// destructor
EEPROMHandler::~EEPROMHandler(){
    if (eepromutil){
      delete eepromutil;         
      eepromutil = NULL;    
    }
    EEPROM.end();    
}

bool EEPROMHandler::save(){

      String data = "";
      serializeJson(jsonDoc, data);
                
      Serial.print("Data to save to EEPROM: ");
      Serial.println(data);
      write_StringEE(0, data);
        
}

bool EEPROMHandler::updateProperty(String property, String value, boolean saveValues){
      this->jsonDoc[property] = value;
      if (saveValues) {
        return save();
      } else {
        return true;
      }
}

// This method should call before every getParameter() call to get the update config from eeprom
void EEPROMHandler::load(){
      
      String savedData = read_StringEE(0, 500);      

      DeserializationError error = deserializeJson(jsonDoc, savedData);

      if (error) {
        Serial.println("DeserializationError: ");
        Serial.println(error.c_str());
      } else {
        Serial.println("EEPROM data:");
        serializeJsonPretty(jsonDoc, Serial);
      }
}

String EEPROMHandler::getValueAsString(String name, bool loadbefore) {
      if (loadbefore){
        load();
      }
      
      String value = jsonDoc[name.c_str()].as<String>();
      
      //Serial.println("Get value as String.\nName: \"" + name + "\"\nvalue: \"" + value + "\"");      
      //Serial.println("Return value: " + value + "\n-------------");
      if (!value || value == "null") {
        value = "";
      }
           
      return value;
}

int EEPROMHandler::getValueAsInt(String name, bool loadbefore) {
      if (loadbefore){
        load();
      }
      
      int ret = -1;
      String value = jsonDoc[name.c_str()].as<String>();

      /* 
         In case of interval the default value should be 1000ms
         Set this value to prevent user typo
      */

      if (name == "interval"){
        ret = 1000;
      }
      
      
      //Serial.println("Get value as int.\nName: \"" + name + "\"\nvalue: \"" + value + "\"");                  
      if (IsNumeric(value)) {
        ret = value.toInt();
      }
      //Serial.println("Return value: " + String(ret) + "\n-------------");      
      return ret;
}
    
void EEPROMHandler::reset(){
      // Reset settings
      Serial.println("clearing eeprom");
      for (int i = 0; i < 512; ++i) { EEPROM.write(i, 0); }
      EEPROM.commit();
      Serial.println("eeprom is clean");
}

void EEPROMHandler::restart(){
      Serial.println("ESP is restarting...");
      ESP.restart();
}
    
// Private methods
bool EEPROMHandler::write_StringEE(int Addr, String input) {        
      char cbuff[input.length()+1];//Finds length of string to make a buffer
      input.toCharArray(cbuff,input.length()+1);//Converts String into character array
      bool writedone = eepromutil->eeprom_write_string(Addr,cbuff);//Saves String
      EEPROM.commit();      
      return writedone;  
}

String EEPROMHandler::read_StringEE(int Addr, int length) {
      EEPROM.begin(512);
      char cbuff[length+1];
      eepromutil->eeprom_read_string(Addr, cbuff, length+1);      
      String stemp(cbuff);
      EEPROM.commit();     
      return stemp;      
}

boolean EEPROMHandler::IsNumeric(String str) {
      unsigned int stringLength = str.length();
   
      if (stringLength == 0) {
          return false;
      }
   
      boolean seenDecimal = false;
   
      for(unsigned int i = 0; i < stringLength; ++i) {
          if (isDigit(str.charAt(i))) {
              continue;
          }
   
          if (str.charAt(i) == '.') {
              if (seenDecimal) {
                  return false;
              }
              seenDecimal = true;
              continue;
          }
          return false;
      }
      return true;
}

/*
void EEPROMHandler::splitString(String text, char splitChar, String result[]) {
    int splitCount = countSplitCharacters(text, splitChar);    
    char buff[text.length()+1];
    text.toCharArray(buff,text.length()+1);
    int counter = 0;
    int from = 0;    

    for(int i = 0; i < sizeof(buff)-1; i++) {        
        if (splitCount < MAX_RESULT_NUMBER){          
          if (buff[i] == splitChar){
            result[counter] = text.substring(from, i);
            result[counter].trim();            
            //Serial.println("Values in EEPROM:");
            //Serial.print(counter);
            //Serial.print(": ");
            //Serial.println(result[counter]);
            from = i+1;
            counter++;
          }
        } else {
          Serial.print("Warning! The max result number is smaller than the given data. Need to modify the source code. Max result number is ");
          Serial.println(MAX_RESULT_NUMBER);            
          break;
        }        
    }
}

int EEPROMHandler::countSplitCharacters(String text, char splitChar) {
    int returnValue = 0;
    char buff[text.length()+1];
    text.toCharArray(buff,text.length()+1);

    for(int i = 0; i < sizeof(buff)-1; i++){      
      if (buff[i] == splitChar){
        returnValue++;
      }
    }    
    return returnValue;
} 
*/
