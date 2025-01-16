/*    
    OctoPrint OLED Display - Basic Example


    This example shows how to use an OLED screen (SSD1306) to display the status of a 3D printer connected to an OctoPrint server.

*/

#include <Arduino.h>
#include <WiFi.h>
#include <PrinterManager.h>
#include <OctoPrintOledDisplay.h>


// OctoPrint server configuration
// replace with your OctoPrint server IP and API key
PrinterManager printerManager("OCTOPRINT_IP", "OCTOPRINT_APIKEY");
// instance of OctoPrintOledDisplay
OctoPrintOledDisplay oledDisplay(printerManager);


// WiFi connection setup
void setupWiFi() {
    Serial.println("Connecting to WiFi...");
    // replace with your network credentials
    WiFi.begin("WIFI_SSID", "WIFI_PASSWORD");
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("Connected to WiFi!");
}

void setup() {
    Serial.begin(9600);
    setupWiFi();
    oledDisplay.setup();

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
