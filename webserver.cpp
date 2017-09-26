#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "eepromhandler.h"

// This is a WebServer implementation
// Including, web template

class WebServer {

  ESP8266WebServer* server;
  String content;  
  EEPROMHandler* eepromhandler;
  String* networks;
  String display = "station"; // station/ap

  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  char jsonChar[500];
    
  public:    
    WebServer(ESP8266WebServer& server, EEPROMHandler& eepromhandler){
       this->server = &server;
       this->eepromhandler = &eepromhandler;       
    }

    void setup(bool wifi_station_mode, String* networks) {      
      createWebServer();
      server -> begin();
      this->networks = networks;      
      Serial.print("WebServer started as ");
      if (wifi_station_mode) {
        Serial.println("station mode");
        String display = "station"; // default
      } else {
        Serial.println("AP mode");
        display = "ap";
      }
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
      
        server->on("/", [=]() {  
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
            

            if (action == "updatesettings") {

              root["device"] = device;
              root["room"] = room;
              root["button"] = button;
              root["btype"] = btype;
              root["relay"] = relay;              
              root["dht22"] = dht22;
              root["led"] = led ;
              root["resist"] = resist;
              root["rtype"] = rtype;
              root["interval"] = interval;
              root["resistname"] = resistname;
              root["trigger"] = trigger;
              root["echo"] = echo;
              root["ssid"] = ssid;
              root["wifipasswd"] = wifipasswd;
              root["mqttbroker"] = mqttbroker;
              root["mqttuser"] = mqttuser;
              root["mqttpw"] = mqttpw;
              
              eepromhandler->save(root);
            }

            eepromhandler->load();
            
            // Always send the same conent with an updated data in it
            content =  "<!DOCTYPE HTML><html>\r\n";
            content += " <head>\r\n";
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
            content += "<button type=\"button\" class=\"btn btn-warning\" onclick=\"location.href='/restart'\">Reboot  (reload config)</button> <button type=\"button\" class=\"btn btn-danger\" onclick=\"location.href='/reset'\">Reset (clear config)</button>";
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

        server->on("/reset", [=]() {

            eepromhandler->reset();
          
            content =  "<!DOCTYPE HTML><html>\r\n";
            content += " <head>\r\n";
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

        server->on("/restart", [=]() {
        
          
            content =  "<!DOCTYPE HTML><html>\r\n";
            content += " <head>\r\n";
            content += " <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">\r\n";
            content += " </head>\r\n";
            content += " <body>\r\n";
            content += " <div class=\"container\">\r\n";
            content += "  <div class=\"row\">\r\n";
            content += "    <div class=\"col-lg-12 col-centered\">\r\n";
            content += "      <h2>Restart was successful! Please wait some seconds to complete the reboot.</h2>\r\n";
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
    }
};
