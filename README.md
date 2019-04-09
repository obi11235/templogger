# templogger

A NodeMCU (ESP8266) based temperature and humidity logger that submits metrics to Datadog. Written with the Arduino IDE in C++.

## Some features:

* Uses the Datadog API to submit tagged metrics via HTTPS once per minute.
* Saves configuration data (including SSID password and Datadog API key) to EEPROM instead of source code.
* Serves a web page with status (Datadog tags), temperature, and humidity pages.
* Operational information via Arduino IDE Serial Monitor and diagnostic lights (optional lights out mode).
* Networked temperature/humidity sensors for about $13 each!

## Project directory:

templogger_serial - Setup Wifi and templogger application via serial monitor to input configuration data. The config is then saved to the EEPROM. The serial connection is no longer required for normal operation. Just apply power and the application runs.

templogger_accesspoint - Work in progress. NodeMCU will act as a Wifi Access Point and allow local operators to connect to a temporary SSID for the purposes of configuring the Wifi and templogger application.

templogger_simple - Not actively developed.

## Arduino IDE dependencies:

Add board via File > Preferences - Additional Board Manager URL and paste in:
http://arduino.esp8266.com/stable/package_esp8266com_index.json

Go to Tools > Board > Board Manager and select your board (ex. NodeMCU 1.0 ESP-12E Module)

Install USB-Serial driver for your operating system and USB-Serial chip (ex. CP2102 USB-Serial chip)
https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

Install sensor libraries via Tools > Manage Libraries:
1. Adafruit Unified Sensor by Adafruit (tested with version 1.0.3)
2. DHT sensor library by Adafruit (tested with version 1.3.4)
3. DHT sensor library for ESPx by beegee_tokyo (tested with version 1.0.9)

## Arduino IDE Serial Monitor settings

Go to Tools > Port and select your port (ex. /dev/cu.SLAB_USBtoUART)

After plugging in the board to USB go to Serial Monitor and select the line ending and baud rate options.
* Newline
* 74880 baud

## DHT sensor variations:
You may use DHT11 or DHT22. You will have to update the code to reflect the model and the GPIO pin you're using for your sensor. templogger_serial code is currently set to DHT11 with the following wiring positions: Vin -> +, GND -> -, GPIO5 -> out

Code snippet:
```
#define DHTTYPE DHT11
#define DHTPIN  5
```

## Uploading templogger to NodeMCU / initial configuration

After making the adjustments above, click upload in the main IDE window, and wait for the flashing process to finish. The NodeMCU will restart and you will be prompted to update the configuration. Enter the requested information. When finished you should have a working application, or at least enough information from the serial monitor to troubleshoot the issue.

## Serial Monitor output
When running properly, you'll see output like this on the serial monitor:
```
Temperature: 27 C
Humidity: 25%
sending NTP packet...
1554837085
connecting to api.datadoghq.com:443
request sent
{"status": "ok"}
closing connection
```

## Hardware in use:
https://www.amazon.com/HiLetgo-Internet-Development-Wireless-Micropython/dp/B010N1SPRK/ref=sr_1_1?keywords=hiletgo+esp8266&qid=1554826323&s=hi&sr=1-1-catcorr

https://www.amazon.com/HiLetgo-Temperature-Humidity-Arduino-Raspberry/dp/B01DKC2GQ0/ref=sr_1_1?keywords=hiletgo+dht11&qid=1554826409&s=hi&sr=1-1

## Diagnostic LEDs
LED near USB port:
* Blink - Datadog acknowledged receipt of either the temperature or the humidity metric

LED near Wifi antenna:
Approximately 20 seconds after power on:
* Off - no connection to Wifi SSID
* On - connected to Wifi SSID

Both LEDs:
* Off - application has not been successfully flashed or power problems
