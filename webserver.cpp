#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <LinkedList.h>
#include "eepromhandler.h"

// This is a WebServer implementation
// Including, web template

class WebServer {

    ESP8266WebServer* server;
    String content;
    EEPROMHandler* eepromhandler;
    LinkedList<String>* networks;
    String display = "station"; // station/ap
    char jsonChar[1000];

  public:
    WebServer(ESP8266WebServer& server, EEPROMHandler& eepromhandler) {
      this->server = &server;
      this->eepromhandler = &eepromhandler;
    }

    void setup(bool wifi_station_mode, LinkedList<String>& networks) {

      this->networks = &networks;
      Serial.print("WebServer started in ");
      if (wifi_station_mode) {
        Serial.println("station mode");
        String display = "station"; // default
      } else {
        Serial.println("AP mode");
        display = "ap";
      }

      createWebServer();
      server -> begin();
    }

    void loop() {
      server -> handleClient();
    }

  private:
    /*
      private methods comes here
      void privatefunc(){

      }
    */

    void createWebServer() {

      if (display == "station") {
        server->on("/", [ = ]() {
          // If there is parameter "updatesettings", save the data
          // otherwise  just read the data from EEPROM and send it with the content
          //?device=&room=&button=&btype=stateful&relay=&dht22=&led=&resist=&rtype=analog&interval=

          String action = server->arg("action") == NULL ? "" : server->arg("action");

          String device = server->arg("device") == NULL ? "" : server->arg("device");
          String room = server->arg("room") == NULL ? "" : server->arg("room");
          String button = server->arg("button") == NULL ? "" : server->arg("button");
          String btype = server->arg("btype") == NULL ? "" : server->arg("btype");
          String relay = server->arg("relay") == NULL ? "" : server->arg("relay");
          String dht22 = server->arg("dht22") == NULL ? "" : server->arg("dht22");
          String led = server->arg("led") == NULL ? "" : server->arg("led");
          String resistname = server->arg("resistname") == NULL ? "" : server->arg("resistname");
          String resist = server->arg("resist") == NULL ? "" : server->arg("resist");
          String rtype = server->arg("rtype") == NULL ? "" : server->arg("rtype");
          String interval = server->arg("interval") == NULL ? "" : server->arg("interval");
          String trigger = server->arg("trigger") == NULL ? "" : server->arg("trigger");
          String echo = server->arg("echo") == NULL ? "" : server->arg("echo");
          String ssid = server->arg("ssid") == NULL ? "" : server->arg("ssid");
          String wifipasswd = server->arg("wifipasswd") == NULL ? "" : server->arg("wifipasswd");
          String mqttbroker = server->arg("mqttbroker") == NULL ? "" : server->arg("mqttbroker");
          String mqttuser = server->arg("mqttuser") == NULL ? "" : server->arg("mqttuser");
          String mqttpw = server->arg("mqttpw") == NULL ? "" : server->arg("mqttpw");
          String sda = server->arg("sda") == NULL ? "" : server->arg("sda");
          String sdc = server->arg("sdc") == NULL ? "" : server->arg("sdc");
          String motion = server->arg("motion") == NULL ? "" : server->arg("motion");
          String ledpin = server->arg("ledpin") == NULL ? "" : server->arg("ledpin");
          String lednum = server->arg("lednum") == NULL ? "" : server->arg("lednum");
          String txpin = server->arg("txpin") == NULL ? "" : server->arg("txpin");
          String rxpin = server->arg("rxpin") == NULL ? "" : server->arg("rxpin");


          if (action == "updatesettings") {
            eepromhandler->load();

            eepromhandler-> updateProperty("device", device, false);
            eepromhandler-> updateProperty("room", room, false);
            eepromhandler-> updateProperty("button", button, false);
            eepromhandler-> updateProperty("btype", btype, false);
            eepromhandler-> updateProperty("relay", relay, false);
            eepromhandler-> updateProperty("dht22", dht22, false);
            eepromhandler-> updateProperty("led", led , false);
            eepromhandler-> updateProperty("resist", resist, false);
            eepromhandler-> updateProperty("rtype", rtype, false);
            eepromhandler-> updateProperty("interval", interval, false);
            eepromhandler-> updateProperty("resistname", resistname, false);
            eepromhandler-> updateProperty("trigger", trigger, false);
            eepromhandler-> updateProperty("echo", echo, false);
            eepromhandler-> updateProperty("ssid", ssid, false);
            eepromhandler-> updateProperty("wifipasswd", wifipasswd, false);
            eepromhandler-> updateProperty("mqttbroker", mqttbroker, false);
            eepromhandler-> updateProperty("mqttuser", mqttuser, false);
            eepromhandler-> updateProperty("mqttpw", mqttpw, false);
            eepromhandler-> updateProperty("sda", sda, false);
            eepromhandler-> updateProperty("sdc", sdc, false);
            eepromhandler-> updateProperty("motion", motion, false);
            eepromhandler-> updateProperty("ledpin", ledpin, false);
            eepromhandler-> updateProperty("lednum", lednum, false);
            eepromhandler-> updateProperty("txpin", txpin, false);
            eepromhandler-> updateProperty("rxpin", rxpin, false);                      

            eepromhandler->save();
          }

          eepromhandler->load();


          // Always send the same conent with an updated data in it
          content =  "<!DOCTYPE HTML><html>\r\n";
          content += " <head>\r\n";
          content += " <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n";
          content += " <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\r\n";
          content += " <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">\r\n";
          content += " </head>\r\n";
          content += " <body>\r\n";
          content += " <div class=\"container\">\r\n";

          if (action == "updatesettings") {
            content += "<div class=\"alert alert-success\">Configuration is updated in EEPROM. Please restart the device to apply the new values in the firmware!</div>";
          } else {
            content += "<div class=\"alert\">&nbsp;</div>";
          }

          content += "<div class=\"pull-right\">";
          content += "<button type=\"button\" class=\"btn btn-info\" onclick=\"location.href='/info'\">MQTT info</button> <button type=\"button\" class=\"btn btn-warning\" onclick=\"location.href='/restart'\">Reboot  (reload config)</button> <button type=\"button\" class=\"btn btn-danger\" onclick=\"location.href='/reset'\">Reset (clear config)</button>";
          content += "</div>";

          content += "   <form action=\"/\" method=\"get\" class=\"form-horizontal\" role=\"form\">\r\n";
          content += "    <h2>Configuration Form</h2>\r\n";
          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"device\" class=\"col-sm-3 control-label\">Device Name</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"device\" type=\"text\" id=\"device\" placeholder=\"Device Name\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("device", false);
          content += "\" autofocus>\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";
          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"room\" class=\"col-sm-3 control-label\">Room Name</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"room\" type=\"text\" id=\"room\" placeholder=\"Room Name\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("room", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += " </div>\r\n";

          content += "    <hr />\r\n";
          content += "    <h2>Network configuration</h2>\r\n";

          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"ssid\" class=\"col-sm-3 control-label\">SSID</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"ssid\" type=\"text\" id=\"ssid\" placeholder=\"wireless SSID\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("ssid", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"wifipasswd\" class=\"col-sm-3 control-label\">WIFI password</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"wifipasswd\" type=\"password\" id=\"wifipasswd\" placeholder=\"wireless password\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("wifipasswd", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"mqttbroker\" class=\"col-sm-3 control-label\">MQTT broker</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"mqttbroker\" type=\"text\" id=\"mqttbroker\" placeholder=\"MQTT broker address\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("mqttbroker", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"mqttuser\" class=\"col-sm-3 control-label\">MQTT user</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"mqttuser\" type=\"text\" id=\"mqttuser\" placeholder=\"MQTT broker user\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("mqttuser", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"mqttpw\" class=\"col-sm-3 control-label\">MQTT password</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"mqttpw\" type=\"password\" id=\"mqttpw\" placeholder=\"MQTT broker password\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("mqttpw", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <hr />\r\n";
          content += "    <h2>Pin configuration</h2>\r\n";
          content += "    <span class=\"help-block\">pin of the devices (-1 means that device is not available in this device)</span>\r\n";

          content += "    <div class=\"form-group form-inline\">\r\n";
          content += "      <label for=\"button\" class=\"col-sm-3 control-label\">Button</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <input name=\"button\" type=\"text\" id=\"button\" placeholder=\"pin of the button\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("button", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "      <label for=\"btype\" class=\"col-sm-3 control-label\">Button type</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <select  name=\"btype\" id=\"btype\" class=\"form-control\">\r\n";
          content += "          <option>stateful</option>\r\n";
          content += "          <option ";
          content += eepromhandler->getValueAsString("btype", false) == "stateless" ? "selected=\"selected\"" : "";
          content += ">stateless</option>\r\n";
          content += "        </select>\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group form-inline\">\r\n";
          content += "      <label for=\"trigger\" class=\"col-sm-3 control-label\">Distance trigger</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <input name=\"trigger\" type=\"text\" id=\"trigger\" placeholder=\"pin of the trigger\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("trigger", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "      <label for=\"echo\" class=\"col-sm-3 control-label\">Distance echo</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <input name=\"echo\" type=\"text\" id=\"echo\" placeholder=\"pin of the echo\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("echo", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group form-inline\">\r\n";
          content += "      <label for=\"sda\" class=\"col-sm-3 control-label\">Display SDA</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <input name=\"sda\" type=\"text\" id=\"sda\" placeholder=\"pin of the display sda\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("sda", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "      <label for=\"sdc\" class=\"col-sm-3 control-label\">Display SDC/SCL</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <input name=\"sdc\" type=\"text\" id=\"sdc\" placeholder=\"pin of the display sdc/scl\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("sdc", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"relay\" class=\"col-sm-3 control-label\">Relay</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"relay\" type=\"text\" id=\"relay\" placeholder=\"pin of the relay\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("relay", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += " </div>\r\n";

          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"dht22\" class=\"col-sm-3 control-label\">DHT 22</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"dht22\" type=\"text\" id=\"dht22\" placeholder=\"pin of the DHT 22\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("dht22", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"led\" class=\"col-sm-3 control-label\">Led</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input  name=\"led\" type=\"text\" id=\"led\" placeholder=\"pin of the led\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("led", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"motion\" class=\"col-sm-3 control-label\">Motion sensor</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input  name=\"motion\" type=\"text\" id=\"motion\" placeholder=\"pin of the motion sensor\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("motion", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group form-inline\">\r\n";
          content += "      <label for=\"button\" class=\"col-sm-3 control-label\">Led number on strip</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <input name=\"lednum\" type=\"text\" id=\"lednum\" placeholder=\"led number on led strip (WS2812B)\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("lednum", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "      <label for=\"btype\" class=\"col-sm-3 control-label\">Led strip data pin</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <select  name=\"ledpin\" id=\"ledpin\" class=\"form-control\">\r\n";
          content += "          <option value=\"\">None</option>\r\n";
          content += "          <option value=\"4\" ";
          content += eepromhandler->getValueAsString("ledpin", false) == "4" ? "selected=\"selected\"" : "";
          content += "          >4</option>\r\n";
          content += "          <option value=\"5\" ";
          content += eepromhandler->getValueAsString("ledpin", false) == "5" ? "selected=\"selected\"" : "";
          content += "          >5</option>\r\n";
          content += "        </select>\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";


          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"resistname\" class=\"col-sm-3 control-label\">Resist name</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input  name=\"resistname\" type=\"text\" id=\"resistname\" placeholder=\"name of the resist\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("resistname", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group form-inline\">\r\n";
          content += "      <label for=\"text\" class=\"col-sm-3 control-label\">Resist</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <input  name=\"resist\" type=\"text\" id=\"text\" placeholder=\"pin of the water or moisture sensor\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("resist", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "      <label for=\"rtype\" class=\"col-sm-3 control-label\">Resist type</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <select  name=\"rtype\" id=\"rtype\" class=\"form-control\">\r\n";
          content += "          <option>analog</option>\r\n";
          content += "          <option ";
          content += eepromhandler->getValueAsString("rtype", false) == "digital" ? "selected=\"selected\"" : "";
          content += ">digital</option>\r\n";
          content += "        </select>\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";

          content += "    <div class=\"form-group form-inline\">\r\n";
          content += "      <label for=\"txpin\" class=\"col-sm-3 control-label\">Soft serial TX</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <input name=\"txpin\" type=\"text\" id=\"txpin\" placeholder=\"softserial tx pin\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("txpin", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "      <label for=\"rxpin\" class=\"col-sm-3 control-label\">Soft serial RX</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <input name=\"rxpin\" type=\"text\" id=\"rxpin\" placeholder=\"oftserial rx pin\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("rxpin", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";


          content += "    <hr />\r\n";
          content += "    <h2>Time configuration</h2>\r\n";
          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"text\" class=\"col-sm-3 control-label\">Data send interval (ms)</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input name=\"interval\" type=\"text\" id=\"text\" placeholder=\"send interval\" class=\"form-control\" value=\"";
          content += eepromhandler->getValueAsString("interval", false);
          content += "\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";
          content += "    <hr />\r\n";
          content += "    <div class=\"form-group\">\r\n";
          content += "      <div class=\"col-sm-9 col-sm-offset-3\">\r\n";
          content += "        <button type=\"submit\" class=\"btn btn-primary btn-block\">Submit</button>\r\n";
          content += "        <input type=\"hidden\" name=\"action\" value=\"updatesettings\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";
          content += "  </form> <!-- /form -->\r\n";
          content += " </div> <!-- ./container -->\r\n";
          content += " </body>\r\n";
          content += " </html>\r\n";
          server->send(200, "text/html", content);

          // Restart after update settings to reload objects
          if (action == "updatesettings") {
            //eepromhandler->restart();
          }
        });
      }

      if (display == "ap") {
        server->on("/", [ = ]() {

          String action = server->arg("action") == NULL ? "" : server->arg("action");
          String ssid = server->arg("ssid") == NULL ? "" : server->arg("ssid");
          String password = server->arg("password") == NULL ? "" : server->arg("password");

          if (action == "updatewifi") {
            eepromhandler->load();
            eepromhandler-> updateProperty("ssid", ssid, false);
            eepromhandler-> updateProperty("wifipasswd", password, false);
            eepromhandler->save();
          }



          content =  "<!DOCTYPE HTML><html>\r\n";
          content += " <head>\r\n";
          content += " <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n";
          content += " <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\r\n";
          content += " <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">\r\n";
          content += " </head>\r\n";
          content += " <body>\r\n";
          content += " <div class=\"container\">\r\n";

          if (action == "updatewifi") {
            content += "<div class=\"alert alert-success\">Configuration is updated in EEPROM. Please restart the device to apply the new wifi connection!</div>";
          } else {
            content += "<div class=\"alert\">&nbsp;</div>";
          }

          content += "<div class=\"pull-right\">";
          content += "<button type=\"button\" class=\"btn btn-warning\" onclick=\"location.href='/restart'\">Reboot  (reload config)</button>";
          content += "</div>";

          content += "  <div class=\"row\">\r\n";
          content += "   <form action=\"/\" method=\"get\" class=\"form-horizontal\" role=\"form\">\r\n";
          content += "    <div class=\"form-group\">";
          content += "      <label for=\"ssid\" class=\"col-sm-3 control-label\">Network</label>\r\n";
          content += "      <div class=\"col-sm-3\">\r\n";
          content += "        <select name=\"ssid\" id=\"ssid\" class=\"form-control\">\r\n";

          for (int i = 0; i < networks->size(); i++) {
            String net = networks->get(i);
            net.replace("*", "");
            String netName = networks->get(i);
            content += "<option value=\"";
            content += net;
            content += "\">";
            content += netName;
            content += "</option>\r\n";
          }
          content += "        </select>\r\n";
          content += "      </div>\r\n";
          content += "   </div>\r\n";
          content += "    <div class=\"form-group\">\r\n";
          content += "      <label for=\"password\" class=\"col-sm-3 control-label\">Password</label>\r\n";
          content += "      <div class=\"col-sm-9\">\r\n";
          content += "        <input  name=\"password\" type=\"password\" id=\"password\" placeholder=\"wifi password\" class=\"form-control\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";
          content += "    <hr />\r\n";
          content += "    <div class=\"form-group\">\r\n";
          content += "      <div class=\"col-sm-9 col-sm-offset-3\">\r\n";
          content += "        <button type=\"submit\" class=\"btn btn-primary btn-block\">Submit</button>\r\n";
          content += "        <input type=\"hidden\" name=\"action\" value=\"updatewifi\">\r\n";
          content += "      </div>\r\n";
          content += "    </div>\r\n";
          content += "  </form> <!-- /form -->\r\n";
          content += " </div>\r\n";
          content += " </body>\r\n";
          content += " </html>\r\n";
          server->send(200, "text/html", content);
        });

      }

      server->on("/reset", [ = ]() {

        eepromhandler->reset();

        content =  "<!DOCTYPE HTML><html>\r\n";
        content += " <head>\r\n";
        content += " <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n";
        content += " <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\r\n";
        content += " <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">\r\n";
        content += " </head>\r\n";
        content += " <body>\r\n";
        content += " <div class=\"container\">\r\n";
        content += "  <div class=\"row\">\r\n";
        content += "    <div class=\"col-lg-12 col-centered\">\r\n";
        content += "      <h2>Reset was successful, the device is clean!</h2>\r\n";
        content += "      <br/>\r\n";
        content += "      <a href=\"/\">return</a>\r\n";
        content += "    </div>\r\n";
        content += "   </div>\r\n";
        content += " </div>\r\n";
        content += " </body>\r\n";
        content += " </html>\r\n";
        server->send(200, "text/html", content);
      });

      server->on("/restart", [ = ]() {


        content =  "<!DOCTYPE HTML><html>\r\n";
        content += " <head>\r\n";
        content += " <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n";
        content += " <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\r\n";
        content += " <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">\r\n";
        content += " </head>\r\n";
        content += " <body>\r\n";
        content += " <div class=\"container\">\r\n";
        content += "  <div class=\"row\">\r\n";
        content += "    <div class=\"col-lg-12 col-centered\">\r\n";
        content += "      <h2>Restart was successful! Please wait some seconds to complete the reboot and then return to the previous page</h2>\r\n";
        content += "      <br/>\r\n";
        content += "      <a href=\"/\">return</a>\r\n";
        content += "    </div>\r\n";
        content += "   </div>\r\n";
        content += " </div>\r\n";
        content += " </body>\r\n";
        content += " </html>\r\n";
        server->send(200, "text/html", content);

        eepromhandler->restart();
      });

      server->on("/info", [ = ]() {

        String roomInfo = eepromhandler->getValueAsString("room", true);
        String deviceInfo = eepromhandler->getValueAsString("device", true);

        String commandIn = "/home/device/" + roomInfo + "/" + deviceInfo + "/in/";
        String commandOut = "/home/device/" + roomInfo + "/" + deviceInfo + "/";

        content =  "<!DOCTYPE HTML><html>\r\n";
        content += " <head>\r\n";
        content += " <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n";
        content += " <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\r\n";
        content += " <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">\r\n";
        content += " </head>\r\n";
        content += " <body>\r\n";
        content += " <div class=\"container\">\r\n";
        content += "  <div class=\"row\">\r\n";
        content += "    <div class=\"col-lg-12 col-centered\">\r\n";
        content += "      <h2>MQTT info</h2>\r\n";
        content += "      <br/>\r\n";
        content += "        ";

        content += "        <h3>Topic out:</h3>\r\n";
        content += "        <div style=\"margin-left: 20px;\"><strong>" + commandOut + "</strong></div>\r\n";
        content += "        <hr/>\r\n";
        
        content += "        <h3>Led strip color topic</h3>   \r\n";
        content += "          <div style=\"margin-left: 20px;\">Change color topic: <strong>" + commandIn + "color</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Payload: <strong>HEX or RGB color code</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Example: <strong>#FFFF00</strong> or <strong>235,56,88</strong></div>\r\n";
        content += "        <hr/>\r\n";

        content += "        <h3>Led strip brightness topic</h3>   \r\n";
        content += "          <div style=\"margin-left: 20px;\">Change brightness: <strong>" + commandIn + "brightness</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Payload: <strong>number 0-255</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Example: <strong>130</strong></div>\r\n";
        content += "        <hr/>\r\n";

        content += "        <h3>Led strip breathe topic</h3>   \r\n";
        content += "          <div style=\"margin-left: 20px;\">Change brightness: <strong>" + commandIn + "breathe</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Payload: <strong>number 0-10</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">This is working just in case of static color.</div>\r\n";
        content += "        <hr/>\r\n";

        content += "        <h3>Led strip demo mode</h3>   \r\n";
        content += "          <div style=\"margin-left: 20px;\">Change brightness: <strong>" + commandIn + "color</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Payload: <strong>demo01 - demo06</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Animated color modes 1-6</div>\r\n";
        content += "        <hr/>\r\n";

        content += "        <h3>Relay topic</h3>   \r\n";
        content += "          <div style=\"margin-left: 20px;\">Change relay state: <strong>" + commandIn + "relay</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Payload: <strong>1 or 0 (on/off)</strong></div>\r\n";
        content += "        <hr/>\r\n";

        content += "        <h3>Oled display topic</h3>   \r\n";
        content += "          <div style=\"margin-left: 20px;\">Add text topic: <strong>" + commandIn + "text</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Payload: <strong>Text message</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Example: <strong>Hello world!</strong></div>\r\n";
        content += "        <hr/>\r\n";

        content += "        <h3>Soft serial topic</h3>   \r\n";
        content += "          <div style=\"margin-left: 20px;\">Add text topic: <strong>" + commandIn + "serial</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Payload: <strong>Text message</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Example: <strong>Hello world!</strong></div>\r\n";
        content += "          <div style=\"margin-left: 20px;\">Serial speed is 115200 (hardcoded yet)</div>\r\n";
        content += "        <hr/>\r\n";
        
        content += "      <br/>\r\n";
        content += "      <a href=\"/\">return</a>\r\n";
        content += "    </div>\r\n";
        content += "   </div>\r\n";
        content += " </div>\r\n";
        content += " </body>\r\n";
        content += " </html>\r\n";
        server->send(200, "text/html", content);
      });
    }
};
