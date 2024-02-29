#include "WIFI_H.h"

void WiFiManager::connect()
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
}

WiFiManager::Measurement measurement = {"MAC:ADDRESS", 68, 244, 24};

WiFiManager::WiFiManager(char *ssid, char *password)
{
    this->ssid = ssid;
    this->password = password;
    // this->authToken = authToken;
    this->connect();
}
