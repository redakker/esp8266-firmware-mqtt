/**
   setup wifi client or AP mode
   client: if the given credentials are okay to connect to the wifi network, return true
   AP mode: if the given credentials are not fit to the wifi network

   In AP mode the WIFI AP credentials are hardcoded:
   name: ESP_AP_nopw_ip_192_168_1_1
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


    // This linkedList for the scanned network APs. In normal mode does not need this.
    // https://github.com/ivanseidel/LinkedList
    LinkedList<String> networks = LinkedList<String>();


  public:
    OnboardWifi(EEPROMHandler& eepromhandler) {
      this->eepromhandler = &eepromhandler;
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
        Serial.print(password);
        WiFi.persistent(false);

        // Set the hostname from the given device parameters if we have
        // Hostname should be set before Wifi.begin()
        String room = eepromhandler->getValueAsString("room", false);
        String device = eepromhandler->getValueAsString("device", false);

        String hostname = room + "." + device;
        if (hostname == ".") {
          hostname = "NonConfiguredESP";
        }
        WiFi.hostname(hostname);

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
        WiFi.disconnect();
        setupAP();
        this->connected = false;
      } else {
        randomSeed(micros());
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(ipToString(WiFi.localIP()));
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

    LinkedList<String> getNetworks() {
      return this->networks;
    }


  private:

    String ipToString(IPAddress ip) {
      String s = "";
      for (int i = 0; i < 4; i++)
        s += i  ? "." + String(ip[i]) : String(ip[i]);
      return s;
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
          networks.add(ssid);

          delay(10);
        }
        Serial.println("----------------------------------");
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
    /*
      private methods comes here
      void privatefunc(){

      }
    */
};
