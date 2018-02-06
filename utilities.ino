
// Get the current uptime
String uptime()
{
 long days=0;
 long hours=0;
 long mins=0;
 long secs=0;
 secs = millis()/1000; //convect milliseconds to seconds
 mins=secs/60; //convert seconds to minutes
 hours=mins/60; //convert minutes to hours
 days=hours/24; //convert hours to days
 secs=secs-(mins*60); //subtract the coverted seconds to minutes in order to display 59 secs max
 mins=mins-(hours*60); //subtract the coverted minutes to hours in order to display 59 minutes max
 hours=hours-(days*24); //subtract the coverted hours to days in order to display 23 hours max
 //Display results
 Serial.println("Running Time");
 Serial.println("------------");
   if (days>0) // days will displayed only if value is greater than zero
 {
   Serial.print(days);
   Serial.print(" days and :");
 }
 Serial.print(hours);
 Serial.print(":");
 Serial.print(mins);
 Serial.print(":");
 Serial.println(secs);

 return String (days) + " " + String(hours) + ":" + String(mins) + ":" + String(secs);
 
}

void pong(char* topic){
    String topic_str = String(topic);
    
    if (topic_str == String (PING_IN_TOPIC)) {
      // ####################### PUBLISH DEVICE INFO
      StaticJsonBuffer<500> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["fw"] = firmware;        
      root["mac"] = String(WiFi.macAddress());
      root["room"] = room;
      root["device"] = device;
      root["up"] = uptime();
      root["vcc"] = ESP.getVdd33();
          
      char jsonChar[500];
      root.printTo((char*)jsonChar, root.measureLength() + 1);
  
      client.publish(PING_OUT_TOPIC, jsonChar);
    }
}

