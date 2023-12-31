# uselessbox
Useless box - box closes itself after opening by switch

Project is about a so-called useless box. The box has a switch in front. If you flip the switch up, the lid of the box opens and an arm comes out that puts the switch to off position.
And then the arm goes back and the box closes again.

The following hardware is used:

1) ESP8266 controller (D1 mini or similar
![alt text](pictures/esp-processor-module.jpg)
<br>

2) Two small RC servos, one for the lid and one for the arm
![alt text](pictures/servo.jpg)
![alt text](pictures/hobby-servo.png)
<br>

3) A toggle switch
![alt text](pictures/schakelaar.jpg)
<br>

4) A small piece of ledstrip with 6 pixel leds (2812)
![alt text](pictures/ledstrip.jpg)
<br>

5) A small audio amplifier
![alt text](pictures/versterker-audio.jpg)
<br>

6) A small loudspeaker
![alt text](pictures/luidspreker.jpg)
<br>

Load files into the Arduino IDE and save as a new sketch.

Arduino IDE settings:
Under File -> preferences:
(NL: Bestand -> Voorkeuren)
Board Manager URL's: http://arduino.esp8266.com/stable/package_esp8266com_index.json,https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json<br>

Under Tools -> Board -> Board Manager -> ESP8266 by community 3.1.2 install
(NL: Hulpmiddelen -> Board -> Boord Beheer -> ESP8266 by community 3.1.2 install


Board NODEMCU 1.0 ESP-12E Module<br>
Flashsize: 4MB (FS:2MB OTA~: 1019KB)<br>

Libraries needed:
<br>
ESP8266Audio by Earle. F. Philhower Version 1.9.7
<br>
Servo by Arduino 1.2.0
<br>
FastLED by Daniel Garcia Version 3.6.0




