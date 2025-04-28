// main.h
#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h> 
#include <U8g2lib.h>
#include <WiFiUdp.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <NTPClient.h>
#include <ESPAsyncWebServer.h>
#include "Weather.h"
#include "ADC.h"
#include "Storage.h"
#include "Action.h"
#include "WS2812.h"
#include "Display.h"
#include "OneButton.h"

#define SERVO_PIN1  14  //front right
#define SERVO_PIN2  16  //back right
#define SERVO_PIN3  12  //front left
#define SERVO_PIN4  13  //back left

#define OLED_RESET_PIN  U8X8_PIN_NONE  // GPIO
#define OLED_SDA_PIN 4 //GPIO4
#define OLED_SCL_PIN 5 //GPIO5

#define TOUCH_BUTTON_PIN 15     //GPIO15
#define DEBOUNCE_DELAY 40      // Debounce time in milliseconds
#define DOUBLE_CLICK_DELAY 500 // Max delay between clicks for double click
#define LONG_PRESS_TIME 1000   // Time in milliseconds for long press

extern NTPClient timeClient;

extern String temperature; 
extern String humidity; 
extern String weather; 
extern String cityname; 
extern String weatherApiKey;

extern String wifi_ssid;
extern String wifi_pass;

extern float batteryVoltage;
extern int batteryPercentage;

#endif