# HeartsApart
## Where romance <3 meets Arduino

### What you will need:
#### Hardware
- STM32 microcontroller
- ESP 8662 WiFi module
- 4 resistors (1000 ohms)
- 0.91" OLED display 
- ISD1820 voice recorder
- PSR-28N08A-JQ speaker without driver
- a few dozen male-male jumpers, and a few male-female jumpers
- two computers
#### Software
- On one computer, Arduino IDE with the ESP8266WiFi, WiFiClientSecure, PubSubClient, and SoftwareSerial libraries
- On the computer, Keil IDE with the stm32f10x, si2c, oled, hyjk1, delay, and stdio libraries
- HIVE MQ on one computer with Arduino IDE
- Online image-to-byte converter

### How to build
1. Set up a HIVE MQ server and connect to the ESP WiFi module via Arduino
2. Use the image-to-byte convert to display an image of a heart on the OLED display via programming the STM 32 with Keil IDE
3. Programer the heart rate sensor with the STM 32 in Keil IDE such that it receives analog heartbeat readings
4. Connect the TX and RX pins of STM 32 and the ESP module such that the heartbeat measurements can be read by the ESP module and are transmitted to the HIVE MQ server!

