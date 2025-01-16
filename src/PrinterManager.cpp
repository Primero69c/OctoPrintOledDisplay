#include "PrinterManager.h"


/// @brief Constructor for PrinterManager
/// @param ip IP address of octoprint server
/// @param apiKey API key for octoprint server
PrinterManager::PrinterManager(const char* ip, const char* apiKey)
    : _ip(ip), _apiKey(apiKey) {}


/// @brief Current printer status from octoprint
/// @param status reference to a printerstatus object
/// @return true if status retrieval is successful, false otherwise
bool PrinterManager::getStatus(PrinterStatus& status) {

    HTTPClient http;
    String url = String("http://") + _ip + "/api/printer";
    http.begin(url);
    http.addHeader("X-Api-Key", _apiKey);

    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        status.nozzleTemp = doc["temperature"]["tool0"]["actual"].as<float>();
        status.bedTemp = doc["temperature"]["bed"]["actual"].as<float>();

        String jobUrl = String("http://") + _ip + "/api/job";
        http.begin(jobUrl);
        http.addHeader("X-Api-Key", _apiKey);

        if (http.GET() == 200) {
            String jobPayload = http.getString();
            DynamicJsonDocument jobDoc(1024);
            deserializeJson(jobDoc, jobPayload);

            status.state = jobDoc["state"].as<String>();
            status.isPrinting = (status.state == "Printing");
            if (status.isPrinting) {
                status.printProgress = jobDoc["progress"]["completion"].as<float>();
                status.estimatedPrintTime = jobDoc["progress"]["printTimeLeft"].as<int>();
            }
        }

        http.end();
        return true;
    }

    http.end();
    return false;
}
