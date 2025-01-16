// OledDisplay.cpp
// Handles the logic for displaying information on the OLED Screen

#include "OctoPrintOledDisplay.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C // I2C address of the OLED display 

/// @brief Constructor for OledDisplay
/// @param manager reference to the PrinterManager
OctoPrintOledDisplay::OctoPrintOledDisplay(PrinterManager& manager)
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
    printerManager(manager),
    lastSwitchTime(0), 
    switchInterval(5000),
    currentScreen(0),
    numScreens(0),
    switchScreen(false) 
{
        // init screen array
        for (int i = 0; i < MAX_SCREENS; i++)
        {
            screens[i] = nullptr;
        }    
}



// initialize OLED display and default configurations
void OctoPrintOledDisplay::setup() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return;
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.display();

}

void OctoPrintOledDisplay::showMessage(const char* message) {
    display.clearDisplay();
    display.setCursor(0, 16);
    display.println(message);
    display.display();
}

void OctoPrintOledDisplay::showPrintTime()
{   
    
    if(status.estimatedPrintTime > 0)
    {
        int timeLeft  = status.estimatedPrintTime;
        int hours = timeLeft / 3600;
        int minutes = (timeLeft % 3600) / 60;
        int seconds = timeLeft % 60;

        display.setCursor(0, 35);
        display.print("Time Left: ");
        display.print(hours);
        display.print(":");
        if (minutes < 10) display.print("0");
        display.print(minutes);
        display.print(":");
        if (seconds < 10) display.print("0");
        display.print(seconds);
    }
    
    display.display();
}
/// @brief Displays default screen showing current status of the printer
void OctoPrintOledDisplay::showDefaultScreen()
{
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Status: ");
    display.println(status.state);

    if (status.isPrinting) {
        display.setCursor(0, 16);
        display.print("Nozzle: ");
        display.print(status.nozzleTemp, 1);
        display.print("C");

        display.setCursor(0, 24);
        display.print("Bed: ");
        display.print(status.bedTemp, 1);
        display.print("C");

        display.drawRect(0, 43, SCREEN_WIDTH, 8, SSD1306_WHITE);
        int progressWidth = (status.printProgress / 100.0) * (SCREEN_WIDTH - 2);
        display.fillRect(1, 44, progressWidth, 6, SSD1306_WHITE);

        display.setCursor(0, 54);
        display.print("Progress: ");
        display.print(status.printProgress, 1);
        display.print("%");

        showPrintTime();
    } 
    else if (status.state == "Operational") 
    {
        display.setCursor(0, 16);
        display.print("Nozzle: ");
        display.print(status.nozzleTemp, 1);
        display.print("C");

        display.setCursor(0, 32);
        display.print("Bed: ");
        display.print(status.bedTemp, 1);
        display.print("C");
    } 
    else if (status.state == "Paused") 
    {
        showMessage("Paused");
    } 
    else if (status.state == "Canceling") 
    {
        showMessage("Canceling...");
    } 
    else if (status.state == "Offline") 
    {
        showMessage("Offline\nCheck Connection");
    }

    display.display();
}

/// @brief Adds a new screen drawing function to the list. Add custom screens layout to the OLED display.
/// @param drawScreen function pointer to the drawing function for the screen
void OctoPrintOledDisplay::addScreen(ScreenDrawFunction drawScreen)
{
    if (numScreens < MAX_SCREENS)
    {
        screens[numScreens++] = drawScreen;
    }
}

/// @brief Updates the display with the current printer's status. 
/// If multiple screens have been added, this function will switch between them
void OctoPrintOledDisplay::updateStatus() {

    // current status
    printerManager.getStatus(status);


    unsigned long currentTime = millis();

    if (switchScreen && numScreens > 0) {
        if (currentTime - lastSwitchTime > switchInterval) {
            
            if (currentScreen >= numScreens) 
            {
                currentScreen = 0; 
            } 
            else
            {
                currentScreen++; 
            }
            lastSwitchTime = currentTime;
            
        }
    }
    display.clearDisplay();
    
    if (currentScreen == 0)
    {
        showDefaultScreen();
    } else 
    {
        screens[currentScreen - 1](display);
        display.display();
    }
}
