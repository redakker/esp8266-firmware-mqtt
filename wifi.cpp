/**
   setup wifi client or AP mode
   client: if the given credentials are okay to connect to the wifi network, return true
   AP mode: if the given credentials are not fit to the wifi network

   In AP mode the WIFI AP credentials are hardcoded:
   name: ESP_nopw_ip_192_168_1_1
   IP: 192.168.1.1
   password: no password
*/
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "eepromhandler.h"
#include <LinkedList.h>

class OnboardWifi {

    EEPROMHandler* eepromhandler;

    // Work variables
    bool connected = false;
    String ipAddress = "";

    // This linkedList for the scanned network APs. In normal mode does not need this.
    // https://github.com/ivanseidel/LinkedList
    LinkedList<String>* networks = new LinkedList<String>();


  public:
    OnboardWifi(EEPROMHandler& eepromhandler, LinkedList<String>& networks) {
      this->eepromhandler = &eepromhandler;
      this->networks = &networks;
    }

    void setup(String str_ssid, String str_password) {
      char* ssid = const_cast<char*>(str_ssid.c_str());
      char* password = const_cast<char*>(str_password.c_str());
      int c = 0;
      int tryCounter = 20;
      delay(10);

      // We start by connecting to a WiFi network id the given ssid's length is okay
      if (strlen(ssid) != 0) {
        Serial.println();
        Serial.print("Connecting to ");
        Serial.print(ssid);
        Serial.print(" with password ");
        Serial.println(password);
        WiFi.persistent(false);
        WiFi.softAPdisconnect();
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        delay(100);

        // Set the hostname from the given device parameters if we have
        // Hostname should be set before Wifi.begin()
        String room = eepromhandler->getValueAsString("room", false);
        String device = eepromhandler->getValueAsString("device", false);

        String hostname = room + "_" + device;
        Serial.print("Calculated hostname: ");
        Serial.println(hostname);
        if (hostname != "_") {          
          WiFi.hostname(hostname);
        }
        

        WiFi.begin(ssid, password);

        while ( c < tryCounter ) {
          if (WiFi.status() == WL_CONNECTED) {
            break;
          }
          delay(500);
          Serial.print(".");
          c++;
        }
        Serial.println("");
      }

      if (c == tryCounter || strlen(ssid) == 0) {
        Serial.println("Wifi connection was unsuccesful. Setup access point.");
        //WiFi.disconnect();
        setupAP();
        this->connected = false;
      } else {
        randomSeed(micros());
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        this->ipAddress = ipToString(WiFi.localIP());
        Serial.println(this->ipAddress);
        Serial.println("MAC address: ");
        Serial.println(WiFi.macAddress());
        this->connected = true;
      }
    }

    void loop() {

    }

    bool isConnected() {
      return this->connected;
    }

    String getIPAddress() {
      return this->ipAddress;
    }

    void setupAP() {
      WiFi.mode(WIFI_AP_STA);
      //WiFi.disconnect();
      delay(100);
      Serial.println("site survey");
      int n = WiFi.scanNetworks();
      Serial.println("scan done");
      if (n == 0) {
        Serial.println("no networks found");
      } else {
        Serial.print(n);
        Serial.println(" networks found");

        //networks = new String*[n];

        for (int i = 0; i < n; ++i) {
          // Print SSID and RSSI for each network found
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.print(WiFi.SSID(i));
          Serial.print(" (");
          Serial.print(WiFi.RSSI(i));
          Serial.print(")");
          Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
          String ssid = String (WiFi.SSID(i));
          ssid += WiFi.encryptionType(i) == ENC_TYPE_NONE ? String("") : String("*");
          //Serial.println(*ssid);
          networks->add(ssid);

          delay(10);
        }
        Serial.println("----------------------------------");
      }
      Serial.println("");
      delay(100);

      Serial.print("Chip id:");
      Serial.println(ESP.getChipId());
      

      IPAddress ip(192, 168, 1, 1);
      IPAddress gateway(192, 168, 1, 1);
      IPAddress subnet(255, 255, 255, 0);
      WiFi.softAPConfig(ip, gateway, subnet);
      // const char* ap_ssid = "ESP_192_168_1_1";
      String ssid = "ESP_192_168_1_1_" + String(ESP.getChipId());
      const char* ssid_ap = const_cast<char*>(ssid.c_str());
  
      WiFi.softAP(ssid_ap);
      Serial.println("softap ready");
      IPAddress apIP = WiFi.softAPIP();
      Serial.print("IP address: ");
      Serial.println(apIP);
      Serial.print("AP ssid: ");
      Serial.println(ssid_ap);
    }


  private:

    String ipToString(IPAddress ip) {
      String s = "";
      for (int i = 0; i < 4; i++)
        s += i  ? "." + String(ip[i]) : String(ip[i]);
      return s;
    }


    /*
      private methods comes here
      void privatefunc(){

      }
    */
};
