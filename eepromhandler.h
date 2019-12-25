#ifndef _EEPROMHANDLER_H_
#define _EEPROMHANDLER_H_

#include <string.h>
#include <ArduinoJson.h> // version 6

// http://playground.arduino.cc/Code/EepromUtil
// Before compile should add to the compiler's libraries folder 
#include "EepromUtil.h"

// Make more easy to use EEPROMUtil read this:
// http://mario.mtechcreations.com/programing/write-string-to-arduino-eeprom/

class EEPROMHandler {
  
  EepromUtil* eepromutil;
  StaticJsonDocument<1000> jsonDoc;
  
  public:    
    EEPROMHandler();
	  ~EEPROMHandler();

    bool save();
	  bool updateProperty(String property, String value, boolean saveValues);
    void load();
    String getValueAsString(String name, bool loadbefore);
    int getValueAsInt(String name, bool loadbefore);
    void reset();
    void restart();
        
  private:    

    bool write_StringEE(int Addr, String input);
    String read_StringEE(int Addr, int length);    
    boolean IsNumeric(String str);
    void splitString(String text, char splitChar, String retult[]);
    int countSplitCharacters(String text, char splitChar);
};
#endif
