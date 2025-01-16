#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "PrinterManager.h"

typedef void(*ScreenDrawFunction)(Adafruit_SSD1306& display);


class OctoPrintOledDisplay {
public:
    // Constructor
    OctoPrintOledDisplay(PrinterManager& manager);

    void setup();
    void showMessage(const char* message);
    void updateStatus();
    void showPrintTime();
    void showDefaultScreen();

    void addScreen(ScreenDrawFunction drawScreen);

    /// @brief Enables or disables automatic screen switching functionality
    /// @param enable true to enable, false to disable
    void enableScreenSwitching(bool enable) { switchScreen = enable; }

    /// @brief Sets the time interval between screen switches
    /// @param interval interval Time in milliseconds between screen switches (default set to 5000)
    void setScreenSwitchInterval(int interval) { switchInterval = interval; }

    void setCurrentScreen(int screen) { currentScreen = screen; }



private:
    Adafruit_SSD1306 display;
    PrinterManager& printerManager;
    PrinterStatus status;
    unsigned long lastSwitchTime;
    unsigned long switchInterval;
    bool switchScreen;
    int currentScreen;

    static const int MAX_SCREENS = 3;
    ScreenDrawFunction screens[MAX_SCREENS];
    int numScreens;
};

#endif
