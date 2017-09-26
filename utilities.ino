/**
 * setup wifi client or AP mode
 * client: if the given credentials are okay to connect to the wifi network, return true
 * AP mode: if the given credentials are not fit to the wifi network
 * 
 * In AP mode the WIFI AP credentials are hardcoded:
 * name: ESP_AP_nopw_ip_192_168_1_1
 * IP: 192.168.1.1
 * password: no password
 */

bool setup_wifi(char* ssid, char* password, String* networks) {
  int c = 0;
  delay(10);

  // We start by connecting to a WiFi network id the given ssid's length is okay
  if (sizeof(ssid) > 1){
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.persistent(false);
    WiFi.begin(ssid, password);  
    
    while ( c < 20 ) {
      if (WiFi.status() == WL_CONNECTED) { break;}
      delay(500);
      Serial.print(".");
      c++;
    }
    Serial.println("");    
  }

  if (c == 20 && sizeof(ssid) > 1){
    Serial.println("Wifi connection was unsuccesful. Setup acces point.");
    WiFi.disconnect();
    setupAP(networks);
    return false;
  } else {
    randomSeed(micros());
  
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(ipToString(WiFi.localIP()));
    Serial.println("MAC address: ");
    Serial.println(WiFi.macAddress());
    return true;
  }
}

void setupAP(String* networks) {  
  WiFi.mode(WIFI_AP_STA);
  //WiFi.disconnect();
  delay(100);
  Serial.println("site survey");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0){
    Serial.println("no networks found");    
  } else {    
    Serial.print(n);
    Serial.println(" networks found");

    networks = new String[n];

    
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      String ssid = String (WiFi.SSID(i) + (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "": "*");
      networks[i] = ssid;
      delay(10);
     }
  }
  Serial.println("");  
  delay(100);

  IPAddress ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gateway, subnet);
  const char* ap_ssid = "ESP_AP_nopw_ip_192_168_1_1";  
  
  WiFi.softAP(ap_ssid);
  Serial.println("softap ready");  
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(apIP);
  Serial.print("AP ssid: ");
  Serial.println(ap_ssid);  
}

String ipToString(IPAddress ip) {
  String s = "";
  for (int i = 0; i < 4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

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

