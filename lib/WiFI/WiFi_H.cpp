#include "WIFI_H.h"
#include <VEML3328.h>

// initialise network connection (make sure to use correct WiFi.h libary)
void WiFiManager::connect()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
}

// VERY SMART AND GREEN AND 1337
void WiFiManager::sendData(std::string rgbData, float tempData, float humidityData, const char *id)
{
    StaticJsonDocument<400> doc;

    doc["id"] = id;
    doc["humidity"] = humidityData;
    doc["rgb"] = rgbData;
    // temperature
    doc["temperature"] = tempData;
    // timestamp
    doc["timestamp"] = 0;

    // Serialize JSON to String
    String json;
    serializeJson(doc, json);

    // send json over wifi
    HTTPClient http;
    http.begin("http://10.207.245.103:8080/receive");
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(json);
    Serial.println(httpResponseCode);
    http.end();
}

void WiFiManager::sendData(uint16_t greenData, uint16_t redData, uint16_t blueData, float tempData, float humidityData, const char *id)
{
    StaticJsonDocument<400> doc;

    doc["id"] = id;
    doc["humidity"] = humidityData;
    doc["red"] = redData;
    doc["green"] = greenData;
    doc["blue"] = blueData;
    // temperature
    doc["temperature"] = tempData;
    // timestamp => #TODO
    doc["timestamp"] = 0;

    // Serialize JSON to String
    String json;
    serializeJson(doc, json);

    // send json over wifi
    HTTPClient http;
    http.begin("http://192.168.0.112:8080/recieve");
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(json);
    Serial.println(httpResponseCode);
    http.end();
}

WiFiManager::Measurement measurement = {"MAC:ADDRESS", 68, 244, 24};

WiFiManager::WiFiManager(char *ssid, char *password)
{
    this->ssid = ssid;
    this->password = password;
    // this->authToken = authToken;
    this->connect();
}
