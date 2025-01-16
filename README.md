
The **`OctoPrintOledDisplay`** library provides an easy way to integrate an **ESP32** with an **OLED display** to display 3D printer status from an **OctoPrint** server. It also supports custom screens. 
You can add custom screens if you want to attach more sensors and display those information.

### Features
- Display 3D printer status such as:
  - Nozzle and bed temperature
  - Print progress
  - Estimated print time
  - Printer state (e.g., "Printing", "Paused", "Operational")
  
- Support for custom screens that you can add to display custom information, such as sensor data.
- Allows automatic switching between screens with adjustable intervals.
- Simple to integrate with OctoPrint via API.
  
---
### Install Dependencies
Requirements:
- Wire – for I2C communication
- Adafruit_SSD1306 – to control the OLED display
- Adafruit_GFX – for basic graphical functions
- ArduinoJson – for parsing JSON data from OctoPrint
---

### Example Screens
Basic usage and custom screen example (a custom screen that displays data from an attached DHT sensor)

-[Basic Example](./examples/basic_example/) 

-[Custom Screen with DHT sensor Example](./examples/OctoPrintOledWithDHT/)


# Usage
### Connecting to OctoPrint
Initialize the PrinterManager class with your OctoPrint server's IP address and API key:
PrinterManager printerManager("OCTOPRINT_IP", "OCTOPRINT_APIKEY");

### Connecting to WiFi
Connect ESP32 to a WiFi network:
WiFi.begin("WIFI_SSID", "WIFI_PASSWORD");


