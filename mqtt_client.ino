/*
  Basic ESP8266 MQTT firmware
  author: redman

  Read the documantation on Git:
  https://github.com/redakker/esp8266-firmware-mqtt
 
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <LinkedList.h>
#include "eepromhandler.h"
#include "button.cpp"
#include "relay.cpp"
#include "led.cpp"
#include "dht22.cpp"
#include "resist.cpp"
#include "distance.cpp"
#include "webserver.cpp"

// This linkedList for the scanned network APs. In normal mode does not need this.
// https://github.com/ivanseidel/LinkedList
LinkedList<String> networks = LinkedList<String>();

const char* firmware = "3.11";
const char* mqtt_server = "";
const char* mqtt_user = "";
const char* mqtt_password = "";

const String room = "ROOOOOOOOOOOOOOOOOM";
const String device = "DEVICEEEEEEEEEE";

String commandIn = "/home/device/" + room + "/" + device + "/in";
String commandOut = "/home/device/" + room + "/" + device + "/";
const char* PING_IN_TOPIC = "/home/ping";
const char* PING_OUT_TOPIC = "/home/pong";
bool wifi_station_mode = false;
// MQTT connect try
int lasttry = 10000;

WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);

// Create Objects
EEPROMHandler eepromhandler;
Button button(client, eepromhandler, commandOut);
Relay relay(eepromhandler, commandIn);
Led led;
DHT_22 dht_22(client, eepromhandler, commandOut);
Resist resist(client, eepromhandler, commandOut);
Distance distance(client, eepromhandler, commandOut);

// Webserver
WebServer webserver(server, eepromhandler);

void callback(char* topic, byte* payload, unsigned int length) {

  //####################################################################
  // IMPLEMENT A JSON PARSER, IF YOU WANT TO RECEIVE COMPLICTED MESSGAES
  // https://github.com/bblanchon/ArduinoJson
  //####################################################################

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  /******************************************/
  // Trigger "devices" which can be triggered
  /******************************************/

  // RELAY
  relay.trigger(topic, payload);

  // PING BEGIN
  /// Every device listen a special topic. If it gets this topic, it sends a messgae about its status (MAC, name etc.)
  
  pong(topic);  
  
  // PING END

  led.operation(100,4);
}

void reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    // Wait 5 seconds before retrying
    if (millis() - lasttry > 5000){
      lasttry = millis();
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = room + "/" + device + "-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
        Serial.println("connected");
        // Once connected, publish an announcement...
        client.subscribe(commandIn.c_str());
        client.subscribe(PING_IN_TOPIC);
        led.operation(100,3);      
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
      }
    }
  }
}

// ##################### ARDUIONO setup() and loop()
void setup() {
  // Init serial
  Serial.begin(115200);
  
  // Help to erase EEPROM if something went wrong
  //eepromhandler.reset();

  // Help to erase WiFi persistent stored credentials
  //ESP.flashEraseSector(0x3fe);

  // Read EEPROM and set variables here first in objects as well
  eepromhandler.load();

  /******************************/
  // Call setup methods of objects
  /******************************/

  // BUTTON
  button.setup(eepromhandler.getValueAsInt("button", false), eepromhandler.getValueAsString("btype", false));

  // RELAY
  relay.setup(eepromhandler.getValueAsInt("relay", false));

  // LED
  led.setup(eepromhandler.getValueAsInt("led", false));

  // DHT22 sensor
  dht_22.setup(eepromhandler.getValueAsInt("dht22", false));

  // Resist type sensor (rain, moisture etc.)
  resist.setup(eepromhandler.getValueAsString("resistname", false), eepromhandler.getValueAsInt("resist", false), eepromhandler.getValueAsString("rtype", false));

  // Distance sensor
  distance.setup(eepromhandler.getValueAsInt("trigger", false), eepromhandler.getValueAsInt("echo", false));

  wifi_station_mode = setup_wifi(eepromhandler.getValueAsString("ssid", false), eepromhandler.getValueAsString("wifipasswd", false));
  if (wifi_station_mode) {
    Serial.println("Succesfully connected to the configured network. Wifi started as client mode.");
  } else {
    Serial.println("Cannot connect to the wifi network. Wifi started as AP mode.");
  }

  // Set up WebServer
  webserver.setup(wifi_station_mode, networks);

  if (wifi_station_mode){
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);  
  }
}

void loop() {

  if (!client.connected() && wifi_station_mode) {
    reconnect();
  }

  /******************************/
  // Call loop methods of objects
  /******************************/

  // BUTTON loop
  button.loop();

  // RELAY
  relay.loop();

  // LED
  led.loop();

  // DHT22 sensor
  dht_22.loop();

  // Resist type sensor (rain, moisture etc.)
  resist.loop();

  // Distancesensor
  distance.loop();
  
  // WebServer
  webserver.loop();
  
  // MQTT loop
  if (wifi_station_mode){
    client.loop();
  }
}


