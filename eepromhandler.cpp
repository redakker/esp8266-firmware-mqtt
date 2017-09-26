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

bool EEPROMHandler::save(JsonObject& json){
        
        String  data = json["device"].as<String>(); // [0]
                data+= DELIMITER;
                data+= json["room"].as<String>(); // [1]
                data+= DELIMITER;
                data+= json["button"].as<String>(); // [2]
                data+= DELIMITER;
                data+= json["btype"].as<String>(); // [3]
                data+= DELIMITER;
                data+= json["relay"].as<String>(); // [4]
                data+= DELIMITER;                
                data+= json["dht22"].as<String>(); // [5]
                data+= DELIMITER;
                data+= json["led"].as<String>(); // [6]
                data+= DELIMITER;
                data+= json["resist"].as<String>(); // [7]
                data+= DELIMITER;
                data+= json["rtype"].as<String>(); // [8]
                data+= DELIMITER;                
                data+= json["interval"].as<String>(); // [9]
                data+= DELIMITER;
                data+= json["resistname"].as<String>(); // [10]
                data+= DELIMITER;
                data+= json["trigger"].as<String>(); // [11]
                data+= DELIMITER;
                data+= json["echo"].as<String>(); // [12]
                data+= DELIMITER;
                data+= json["ssid"].as<String>(); // [13]
                data+= DELIMITER;
                data+= json["wifipasswd"].as<String>(); // [14]
                data+= DELIMITER;
                data+= json["mqttbroker"].as<String>(); // [15]
                data+= DELIMITER;
                data+= json["mqttuser"].as<String>(); // [16]
                data+= DELIMITER;
                data+= json["mqttpw"].as<String>(); // [17]
                data+= DELIMITER;
                
                Serial.print("Data to save to EEPROM: ");
                Serial.println(data);
                write_StringEE(0, data);
        
}

// This method should call before every getParameter() call to get the update config from eeprom
void EEPROMHandler::load(){
      String result[MAX_RESULT_NUMBER];      
      String savedData = read_StringEE(0, 500);      
      splitString(savedData, DELIMITER, result);

      root["device"] = result[0]; // [0]
      root["room"] = result[1]; // [1]
      root["button"] = result[2]; // [2]
      root["btype"] = result[3]; // [3]
      root["relay"] = result[4]; // [4]      
      root["dht22"] = result[5]; // [5]
      root["led"] = result[6]; // [6]
      root["resist"] = result[7]; // [7]
      root["rtype"] = result[8]; // [8]
      root["interval"] = result[9]; // [9]
      root["resistname"] = result[10]; // [10]
      root["trigger"] = result[11]; // [11]
      root["echo"] = result[12]; // [12]
      root["ssid"] = result[13]; // [13]
      root["wifipasswd"] = result[14]; // [14]
      root["mqttbroker"] = result[15]; // [15]
      root["mqttuser"] = result[16]; // [16]
      root["mqttpw"] = result[17]; // [17]
}

String EEPROMHandler::getValueAsString(String name, bool loadbefore) {
      if (loadbefore){
        load();
      }
      
      String value = root[name.c_str()].as<String>();
      
      //Serial.println("Get value as String.\nName: \"" + name + "\"\nvalue: \"" + value + "\"");      
      //Serial.println("Return value: " + value + "\n-------------");      
      return value;
}

char EEPROMHandler::getValueAsChar(String name, bool loadbefore) {
      if (loadbefore){
        load();
      }
      
      char value = root[name.c_str()].as<char>();
      
      return value;
}

int EEPROMHandler::getValueAsInt(String name, bool loadbefore) {
      if (loadbefore){
        load();
      }
      
      int ret = -1;
      String value = root[name.c_str()].as<String>();

      /* 
         In case of interval the default value should be 1000ms
         Set this value to prevent user typo
      */

      if (name == "interval"){
        ret = 1000;
      }
      
      
      //Serial.println("Get value as int.\nName: \"" + name + "\"\nvalue: \"" + value + "\"");                  
      if (IsNumeric(value)) {
        ret = root.get<signed int>(name.c_str());
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
