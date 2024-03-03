#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

class WiFiManager
{
public:
    struct Measurement
    {
        const char *id;
        int humidity;
        int color;
        int temp;
    };

    WiFiManager(char *ssid, char *password);
    void connect();
    void sendData(uint16_t greenData, uint16_t redData, uint16_t blueData, float tempData, float humidityData, const char *id);
    void sendData(std::string rgbData, float tempData, float humidityData, const char *id);

private:
    char *ssid;
    char *password;
    char *authToken;
};

#endif // WIFI_MANAGER_H