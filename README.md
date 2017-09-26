# esp8266-firmware-mqtt
A common firmware for ESP8266 chips, to handle mqtt events (send, receive)
I use it to build a clever home.

After download
  - rename steup.h.sample to setup.h
  - add value to variables 
  - from the lib folder copy the files to the compiler (Arduino IDE) libraries folder
and then you can compile it.
  - modify the ESP.h and ESP.cpp to get the microcontorller voltage. Details in the beginig of mqtt_client.ino

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
  - use the configuration for room and device name (forget tom implement :))

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
