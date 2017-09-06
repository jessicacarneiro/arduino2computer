# arduino2computer

This project shows an example of communication between a computer and an Arduino over WiFi using the [Arduino WiFi Shield](https://www.arduino.cc/en/Guide/ArduinoWiFiShield).

## Arduino

You can compile it using [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) by setting the attributes in the  Makefile inside `arduino/` (board configurations, paths, etc.).

`make upload && make monitor`

Or you can compile and upload it inside the Arduino IDE.


## Computer

Use `make` to compile the code and `make run` to compile and run. 
