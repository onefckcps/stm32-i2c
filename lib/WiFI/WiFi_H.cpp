#include "WIFI_H.h"

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

void WiFiManager::sendData(uint16_t greenData, uint16_t redData, uint16_t blueData, float tempData, float humidityData, const char *id)
{
    // // evaluate data
    // uint8_t red = readRed();
    // uint8_t green = readGreen();
    // uint8_t ir = readIr();

    StaticJsonDocument<400> doc;

    doc["id"] = id;
    doc["humidity"] = humidityData;
    doc["red"] = redData;
    doc["green"] = greenData;
    doc["blue"] = blueData;
    // temperature
    doc["temperature"] = tempData;
    // timestamp
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
