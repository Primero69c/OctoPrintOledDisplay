#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct PrinterStatus {
    bool isPrinting;
    float printProgress;
    int estimatedPrintTime;
    float nozzleTemp;
    float bedTemp;
    String state;
};

class PrinterManager {
public:
    
    PrinterManager(const char* ip, const char* apiKey);
    
    bool getStatus(PrinterStatus& status);

private:
    const char* _ip;
    const char* _apiKey;
};

#endif
