#include "..\include\Storage.h"

const char* ssidFile = "/ssid.json";

bool loadHtmlFile(const char* filename, String& content)
{
    if (LittleFS.exists(filename)) 
    {
        fs::File file = LittleFS.open(filename, "r");  // Open file
        if (file) {
            //size_t fileSize = file.size(); 
            content = "";
            while (file.available()) {
                content += (char)file.read();
            }
            file.close(); 
            return true;
        }
        return false;
    } 
    return false;
}

void printWiFiConfig()
{
    Serial.printf("SSID: %s\n",wifi_ssid.c_str());
    Serial.printf("Password: %s\n", wifi_pass.c_str());
    Serial.printf("City: %s\n", cityname.c_str());
    Serial.printf("Key: %s\n", weatherApiKey.c_str());
}

bool loadWiFiConfig()
{
    if (LittleFS.begin())
    {
        fs::File file = LittleFS.open(ssidFile, "r");
        if (file)
        {
            Serial.println("SSID file exists");
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, file);
            if (!error)
            {
                String ssid = doc["ssid"];
                String pass = doc["password"];
                String city = doc["city"];
                String key = doc["key"];
                wifi_ssid = ssid;
                wifi_pass = pass;
                cityname = city;
                weatherApiKey = key;
                printWiFiConfig();
            }
            file.close();
            return true;
        }
        Serial.println("SSID file does not exist.");
        return false;
    }
    return false;
}

bool saveWiFiConfig()
{
    Serial.println("Saving WiFi config:");
    printWiFiConfig();
    JsonDocument doc;
    doc["ssid"] = wifi_ssid;
    doc["password"] = wifi_pass;
    doc["city"] = cityname;
    doc["key"] = weatherApiKey;
    
    // 序列化为字符串
    // String output;
    // serializeJson(doc, output);
    // Serial.println(output);
  
    fs::File file = LittleFS.open(ssidFile, "w");  // 打开文件进行写入
    if (file) {
        serializeJson(doc, file);  // 将JSON内容写入文件
        file.close();
        Serial.println("WiFi config saved.");
        return true;
    }
    return false;
}

