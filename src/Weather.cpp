#include "..\include\Weather.h"
#include "image.cpp"

const char* weatherAPI = "http://api.seniverse.com/v3/weather/daily.json?key="; // Weather API URL
bool initweather = false;           

void fetchWeather()
{
    if (initweather == false)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            WiFiClient client;
            HTTPClient http;
            if (http.begin(client, weatherAPI + weatherApiKey + "&location=" + cityname + "&language=zh-Hans&unit=c&start=0&days=1"))
            {
                int httpCode = http.GET();
                if (httpCode > 0)
                {
                    String payload = http.getString();
                    Serial.println("JSON Response:");
                    Serial.println(payload);
                    JsonDocument doc;
                    deserializeJson(doc, payload);
                    String temperature2 = doc["results"][0]["daily"][0]["high"];
                    String humidity2 = doc["results"][0]["daily"][0]["humidity"];
                    String weathe2r = doc["results"][0]["daily"][0]["text_day"];
                    temperature = temperature2;
                    humidity = humidity2;
                    weather = weathe2r;
                    initweather = true;
                    Serial.print("Data received: ");
                    Serial.println(temperature);
                    Serial.println(humidity);
                    Serial.println(weather);
                }
                else
                {
                    Serial.printf("HTTP GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
                }
                http.end();
            }
            else
            {
                Serial.println("Failed to connect to server");
            }
        }
    }
    
}