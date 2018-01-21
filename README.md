# esp8266-firmware-mqtt
A common firmware for ESP8266 chips, to handle mqtt events (send, receive)
I use it to build a clever home.

This firmware created in Arduino IDE. Use that to compile and upload to the microcontorller.
It has own web config userinterface.
Steps to config the application:
  - upload a firmware to the board
  - it starts in AP mode. Search for ESP_AP_nopw_ip_192_168_1_1 (no password, ip: 192.168.1.1)
  - choose you wifi and type the password, press submit
  - reboot the board
  - it should run in normal (station) mode
  - get the device IP from your router
  - set the config
  - reboot the board
  - have fun

Steps to make it work:

  - download Arduino IDE, https://www.arduino.cc/en/main/software
  - install boards width ESP8266 chip, http://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/
  - open, the code (mqtt_client.ino)
  - change the board (tool/Board/Generic ESP8266 Module)
  - install library: PubSubClient (2.6.0)
  - install library: ArduinoJson (5.11.1)
  - install library: DHT sensor library (1.3.0)
  - install library: Adafruit Unified Sensor (1.0.2)
  - install library: LinkedList (1.2.3)
  
  Search for ESP.cpp in the Arduino ide library: c:\Users\$USER\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266 
  
  
  Modify ESP.cpp the files like:
  ```
  extern "C" uint16_t readvdd33(void);
  uint16_t EspClass::getVdd33(void)
  {
  return readvdd33();
  }
  ```
  
  and ESP.h
  
  after 
  "class EspClass {
    public:" add
  ```
  uint16_t getVdd33(void); 
  ```

Help for some boards, devices

I/O definition (-1 if that is not provided in the current board)

-= ESP-01s =-

Built in led: 2 (use a constant of Arduino: LED_BUILTIN)
Recommended pin to use button: 0

-= Sonoff =-

Built in led: 13
Relay: 12
digital pin: 14 (not tried)


Release notes:

3.60
  - WS2812B led strip implementation
  - on the web interface you just set the data pin of the led strip and the pin number and you can control the led color and brightness wit MQTT command
  - MQTT commands
    - color: 
      - command: /home/devices/[roomname]/[devicename]/in/color
      - values: hexa color code. Example: #FF0000 or FF0000
    - brightness:
      - command: /home/devices/[roomname]/[devicename]/in/brightness
      - valies: 0-255
  - limitation: you can use pin 4 and 5 only, because of this value needs to be constant in the code. So it is solved by if conditions and the code was written in this way.

3.55
  - motion sencor object added (PIR sensor for arduino)

3.53
  - relay fix: the topic condition was wrong
  - button fix: sent a command just once
  - display fix: charachter numbers

3.51
  - OLED object implemented
  - Tested with this kind of oled module: 0.96" I2C IIC Serial 128X64 128x64 White
  - Object has an AddText() method which displays the given text with a right format. You should
  - Currently displays just the special MQTT messages which was sent directly to the display: "/home/device/{room}/{device}/in/text"

3.40
  - eeprom handler can update just one property instead of rewrite the whole data structure (method implemented, but not used yet)
  - eeprom reading moved from loop() functions to setup(). Use the EEPROM as less as we can. If user change the config (s)he must reboot the board to apply the new values

3.21
  - input fields added to the web config (wifi, mqtt)
  - every config comes from EEPROM
  - nice to have: make it more secure (passwords visible in the URL and input value with firebug)

3.11
  - Web based Wifi configuration is ready
  - firmware goes to soft AP mode if wifi credentials is not found in EEPROM
  - still missing to use devicename from config
  - missing MQTT configuration from Web (no input fields)

3.01 
  - web based microcontroller configuration
  - EEPROM handling with easy functions
  - reset EEPROM
  - restart the microcontroller
  - better object structure
  - more debug message over serial port
  TODO: 
  - If user configure not allowed pins (not investigated deeply), the microcontorller crash (not damage)
  Prevent the misconfiguration.
  - use the configuration for room and device name (forget to implement :))

2.11
  - new objects created for different sensors
  - better object structure
  - more detailed MQTT messages
  
  TODO: create a web interface for configuration

2.01
  - separate devices (button,relay,led) to objects
  - separate utilities to another file
  - minimize the code in the main file
  - pong message changed, it sends a room and a device separated in json object
  - visual message with built in led:
    - device started: 5 flash between 100ms
    - subscribed/reconnected to mqtt topics : 3 flash between 100ms
    - mqtt message received: 4 flash between 100ms
  

some code sources and special thanks

https://www.tautvidas.com/blog/2012/08/distance-sensing-with-ultrasonic-sensor-and-arduino/
http://mario.mtechcreations.com/programing/write-string-to-arduino-eeprom/
http://playground.arduino.cc/Code/EepromUtil

and my co-worker Zoltán Megyeri
