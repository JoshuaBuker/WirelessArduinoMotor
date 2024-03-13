# Network controlled Motors

ESP8266 Module hosts a network and starts a websocket server.

The python script then connects to that websocket and sends joystick values over to the ESP8266.

The esp8266 then parses the data and sets the motors according to the joystick values.
