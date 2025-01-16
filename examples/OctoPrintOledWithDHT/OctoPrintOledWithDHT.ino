/* 
  OctoPrint OLED Display -- Custom Screen Example
  This example shows how to add a custom screen to toggle between default screen (printer status screen) and DHT sensor screen that shows room environment.

*/

#include <Arduino.h>
#include <WiFi.h>
#include <PrinterManager.h>
#include <OctoPrintOledDisplay.h>

#include "DHT.h" 

// OctoPrint server configuration
// replace with your OctoPrint server IP and API key
PrinterManager printerManager("OCTOPRINT_IP", "OCTOPRINT_APIKEY");
// instance of OctoPrintOledDisplay
OctoPrintOledDisplay oledDisplay(printerManager);

// define DHTPIN and DHTTYPE
#define DHTPIN 25
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// custom screen -- added DHT library to read room environment
void dhtScreen(Adafruit_SSD1306& display)
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature))
  {
    display.setCursor(0, 16);
    display.println("Failed to read from DHT sensor!");
  }

  display.setCursor(0,0);
  display.println("Room Environment");
  
  display.setCursor(0, 16);
  display.print("Room Temp: ");
  display.print(temperature);
  display.print(" C");

  display.setCursor(0, 32);
  display.print("Humidity: ");
  display.print(humidity);
  display.print(" %");

}




void setupWiFi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin("WIFI_SSID", "WIFI_PASSSWORD");
    // replace with your network credentials
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("Connected to WiFi!");
}

void setup() {
    Serial.begin(9600);

    // init DHT
    dht.begin();

    setupWiFi();
    oledDisplay.setup();

    oledDisplay.addScreen(dhtScreen);

    oledDisplay.enableScreenSwitching(true); // default set to false
    oledDisplay.setScreenSwitchInterval(3000); // default set to 5000
}

void loop() {
    PrinterStatus status;
    // get printer status and update display
    if (printerManager.getStatus(status)) {
        oledDisplay.updateStatus();
    } else {
        oledDisplay.showMessage("Error connecting\nto printer");
    }

    delay(200);
}
