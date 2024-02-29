#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
// #include <WiFiClient.h>
// #include <HTTPClient.h> // not needed => conflicting with wifi.h
// #include <ArduinoJson.h>

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
    void sendData(Measurement &measurement);

private:
    char *ssid;
    char *password;
    char *authToken;
};

#endif // WIFI_MANAGER_H