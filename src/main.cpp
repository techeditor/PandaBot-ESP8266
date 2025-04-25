#include "..\include\main.h"
#include "..\include\ADC.h"

ActionController action(100);
WS2812Controller ledStrip;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 8 * 3600, 60000);
AsyncWebServer server(80);
DisplayController display;

const char *ssid = "PandaBot";
const char *password = "";

// Public variables
String temperature = ""; 
String humidity = ""; 
String weather = ""; 
String cityname = ""; 
String wifi_ssid = "";
String wifi_pass = "";
String weatherApiKey = "";

// ADC
ADC batteryADC(A0);  // 创建ADC类实例，使用A0引脚
float batteryVoltage = 0;
int batteryPercentage = 0;

// State
bool freeState = false;           // 定义一个标志来检查是否需要执行动作
Action actionStateSet = DO_IDLE;  // 用于循环设置用

Screen screenState = EMOJI_HI;
ScreenType screenType = SCREEN_TYPE_EMOJI;

enum SetMode {    
  SET_IDLE,
  SET_ACTION,   
  SET_EMOJI,
  SET_INFO,
  SET_LED    
};
SetMode currentSetMode = SET_IDLE;

WS2812Controller::Effect ledState = WS2812Controller::SOLID;

// 按钮事件处理函数
OneButton button(TOUCH_BUTTON_PIN, false, false);
void handleDoubleClick(){
  Serial.print("[DOUBLE CLICK] ");
  switch (currentSetMode) {
    case SET_IDLE:
      Serial.println("");
      break;
    case SET_ACTION:
      freeState = !freeState;
      Serial.print("Change free state: ");
      Serial.println(freeState ? "ON" : "OFF");
      break;
    case SET_EMOJI:
      Serial.println("");
      break;
    case SET_INFO:
      Serial.println("");
      break;
    case SET_LED:
      ledStrip.toggleOnOff();
      Serial.print("Toggle LED to:");
      Serial.println(ledStrip.getState() ? "ON" : "OFF");
      break;
  }
}

void handleClick(){
  Serial.print("[SINGLE CLICK] ");
  switch (currentSetMode) {
    case SET_IDLE:
      Serial.println("");
      break;
    case SET_ACTION:
      actionStateSet = (enum Action)(((int)actionStateSet + 1) % ACTION_COUNT);
      action.setActionState(actionStateSet);
      Serial.print("Change action to: ");
      Serial.println(actionStateSet);
      break;
    case SET_EMOJI:
      switch(screenState) {
          case SCREEN_NONE:
            screenState = EMOJI_HI;
            break;
          case EMOJI_HI:
            screenState = EMOJI_HAPPY;
            break;
          case EMOJI_HAPPY:
            screenState = EMOJI_SAD;
            break;
          case EMOJI_SAD:
            screenState = EMOJI_SICK;
            break;
          case EMOJI_SICK:
            screenState = EMOJI_ANGRY;
            break;
          case EMOJI_ANGRY:
            screenState = EMOJI_FAINT;
            break;
          case EMOJI_FAINT:
            screenState = EMOJI_LOVE;
            break;
          case EMOJI_LOVE:
            screenState = EMOJI_HI;
            break;
          default:
            screenState = EMOJI_HI;
      }
      display.setScreenType(SCREEN_TYPE_EMOJI);
      Serial.print("Change Emoji to: ");
      Serial.println(screenState);
      break;
    case SET_INFO:
      switch (screenState) {
        case SCREEN_NONE:
          screenState = INFO_TIME;
          Serial.println("info_time");
          break;
        case INFO_TIME:
          screenState = INFO_WEATHER;
          Serial.println("info_weather");
          break;
        case INFO_WEATHER:
          screenState = INFO_POWER;
          Serial.println("info_power");
          break;
        case INFO_POWER:
          screenState = INFO_TIME;
          Serial.println("info_time");
          break;
        default:
          screenState = INFO_TIME;
          Serial.println("info_time");
      }
      display.setScreenType(SCREEN_TYPE_INFO);
      Serial.print("Change Info to: ");
      Serial.println(screenState);
      break;
    case SET_LED:
      ledState = (enum WS2812Controller::Effect)(((int)ledState + 1) % WS2812Controller::LED_EFFECT_COUNT);
      if (ledState==WS2812Controller::LED_OFF){
        ledStrip.setOff();
        Serial.println("Set LED off.");
      }else{
        ledStrip.setEffect(ledState); 
        Serial.print("Change LED effect to: ");
        Serial.println(ledStrip.getCurrentEffect());
      }
      break;
  }
}

void handleLongPress(){
  Serial.print("[LONG PRESS] ");
  switch (currentSetMode) {
    case SET_IDLE:
      currentSetMode = SET_ACTION;
      Serial.println("Setting Action from IDLE.");
      display.showSettingTitle("Action");
      break;
    case SET_ACTION:
      currentSetMode = SET_EMOJI;
      Serial.println("Setting EMOJI.");
      display.showSettingTitle("Emoji");
      break;
    case SET_EMOJI:
      currentSetMode = SET_INFO;
      Serial.println("Setting Info.");
      display.showSettingTitle("Info");
      break;          
    case SET_INFO:
      currentSetMode = SET_LED;
      Serial.println("Setting LED.");
      display.showSettingTitle("LED");
      break;
    case SET_LED:
      currentSetMode = SET_ACTION;
      Serial.println("Setting Action.");
      display.showSettingTitle("Action");
      break;
  }
}

// WiFi Route Handler
void handleWiFiRoute()
{
  // 启动服务器
  server.on("/front", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_FRONT);  
              request->send(200, "text/plain", "Front function started"); });
  server.on("/back", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_BACK);   
              request->send(200, "text/plain", "Front function started"); });
  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_LEFT); 
              request->send(200, "text/plain", "Front function started"); });
  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_RIGHT);  
              request->send(200, "text/plain", "Front function started"); });
  server.on("/shake_left_hand", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_SHAKE_LEFT_HAND);  
              request->send(200, "text/plain", "Front function started"); });
  server.on("/shake_right_hand", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_SHAKE_RIGHT_HAND);  
              request->send(200, "text/plain", "Front function started"); });
  server.on("/sit_down", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_SIT_DOWN); 
              request->send(200, "text/plain", "Front function started"); });
  server.on("/lie", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_LIE);
              request->send(200, "text/plain", "Front function started"); });
  server.on("/sleep", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_SLEEP);
              request->send(200, "text/plain", "sleep."); });              
  server.on("/stand", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_STAND);
              request->send(200, "text/plain", "stand"); });              
  server.on("/stretch", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_STRETCH);
              request->send(200, "text/plain", "Stretch."); }); 
  server.on("/swing_left_right", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_SWING_LEFT_RIGHT);
              request->send(200, "text/plain", "swing_left_right."); }); 
  server.on("/swing_front_back", HTTP_GET, [](AsyncWebServerRequest *request)
            { action.setActionState(DO_SWING_FRONT_BACK);
              request->send(200, "text/plain", "swing_front_back."); }); 

  server.on("/emoji_hi", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenState = EMOJI_HI;  
              request->send(200, "text/plain", "succeed."); });
  server.on("/emoji_happy", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenState = EMOJI_HAPPY;  
              request->send(200, "text/plain", "succeed."); });              
  server.on("/emoji_angry", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenState = EMOJI_ANGRY;   
              request->send(200, "text/plain", "succeed."); });
  server.on("/emoji_sad", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenState = EMOJI_SAD;  
              request->send(200, "text/plain", "succeed."); });
  server.on("/emoji_love", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenState = EMOJI_LOVE;  
              request->send(200, "text/plain", "succeed."); });
  server.on("/emoji_sick", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenState = EMOJI_SICK; 
              request->send(200, "text/plain", "succeed."); });
  server.on("/emoji_faint", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenState = EMOJI_FAINT;
              request->send(200, "text/plain", "succeed."); });
  server.on("/emoji_blink", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenState = EMOJI_BLINK;
              request->send(200, "text/plain", "succeed."); });              

  server.on("/info_time", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenType = SCREEN_TYPE_INFO; 
              screenState = INFO_TIME;
              request->send(200, "text/plain", "succeed."); });
  server.on("/info_weather", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenType = SCREEN_TYPE_INFO; 
              screenState = INFO_WEATHER; 
              request->send(200, "text/plain", "succeed."); });
  server.on("/info_power", HTTP_GET, [](AsyncWebServerRequest *request)
            { screenType = SCREEN_TYPE_INFO; 
              screenState = INFO_POWER; 
              request->send(200, "text/plain", "succeed."); });
  server.on("/get_battery_voltage", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(batteryVoltage)); });
  server.on("/get_battery_percentage", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(batteryPercentage)); });
  
  server.on("/connect", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              wifi_ssid = request->getParam("ssid", true)->value();
              wifi_pass = request->getParam("pass", true)->value();
              cityname= request->getParam("city", true)->value();
              weatherApiKey = request->getParam("key", true)->value();
              saveWiFiConfig();
              WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str()); // 连接WiFi
              request->send(200, "text/html", "<h1>Connecting...</h1><a href=''/''>Back</a>"); 
            });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { String fileContent;
              if (loadHtmlFile("/index.html",fileContent))
                request->send(200, "text/html;charset=utf-8", fileContent);
              else 
                request->send(404, "text/plain", "File 'index.html' Not Found"); 
              return;
            });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { String fileContent;
              if (loadHtmlFile("/index.html",fileContent))
                request->send(200, "text/html;charset=utf-8", fileContent);
              else 
                request->send(404, "text/plain", "File 'index.html' Not Found"); 
              return;
            });            
  server.on("/control.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { String fileContent;
              if (loadHtmlFile("/control.html",fileContent))
              {
                  AsyncResponseStream *response = request->beginResponseStream("text/html;charset=utf-8");   //大文件时需要流式返回
                  response->print(fileContent);
                  request->send(response);
              }
              else 
                request->send(404, "text/plain", "File 'control.html' Not Found"); 
              return;
            });

  server.on("/setting.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { String fileContent;
              if (loadHtmlFile("/setting.html",fileContent))
                request->send(200, "text/html;charset=utf-8", fileContent);
              else 
                request->send(404, "text/plain", "File 'setting.html' Not Found"); 
              return;
            });
            
  // LED控制功能
  server.on("/led_on", HTTP_GET, [](AsyncWebServerRequest *request)
            { ledStrip.setOn();
              request->send(200, "text/plain", "LED turned on"); });
              
  server.on("/led_off", HTTP_GET, [](AsyncWebServerRequest *request)
            { ledStrip.setOff();
              request->send(200, "text/plain", "LED turned off"); });
              
  server.on("/led_toggle", HTTP_GET, [](AsyncWebServerRequest *request)
            { ledStrip.toggleOnOff();
              request->send(200, "text/plain", "LED toggled: " + String(ledStrip.getState() ? "ON" : "OFF")); });
              
  server.on("/led_effect", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if(request->hasParam("effect")) {
                String effect = request->getParam("effect")->value();
                if(effect == "solid") {
                  ledStrip.setEffect(WS2812Controller::SOLID);
                } else if(effect == "blinking") {
                  ledStrip.setEffect(WS2812Controller::BLINKING);
                } else if(effect == "breathing") {
                  ledStrip.setEffect(WS2812Controller::BREATHING);
                } else if(effect == "flowing") {
                  ledStrip.setEffect(WS2812Controller::FLOWING);
                } else if(effect == "rainbow") {
                  ledStrip.setEffect(WS2812Controller::RAINBOW);
                }
                request->send(200, "text/plain", "LED effect set to: " + effect);
              } else {
                request->send(400, "text/plain", "Missing effect parameter");
              }
            });
            
  server.on("/led_color", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if(request->hasParam("r") && request->hasParam("g") && request->hasParam("b")) {
                int r = request->getParam("r")->value().toInt();
                int g = request->getParam("g")->value().toInt();
                int b = request->getParam("b")->value().toInt();
                ledStrip.setColor(CRGB(r, g, b));
                request->send(200, "text/plain", "LED color set to RGB: " + String(r) + "," + String(g) + "," + String(b));
              } else {
                request->send(400, "text/plain", "Missing color parameters (r, g, b)");
              }
            });
            
  server.on("/get_ssid_config", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              File file = LittleFS.open("/ssid.json", "r");
              if(file) {
                String content = file.readString();
                file.close();
                request->send(200, "application/json", content);
              } else {
                request->send(404, "text/plain", "ssid.json not found");
              }
            });

  server.begin();   // 启动服务器
}

//=========================================
void setup() {
  
  delay(100);  // 延迟启动，确保系统稳定
  ledStrip.begin(); 
  ledStrip.setOff();
  ledStrip.setColor(CRGB::White);
  
  batteryADC.begin();  // 初始化ADC
  
  Serial.begin(9600);
  Serial.println("");
  Serial.println("PandaBot setup begins...");

  action.begin();   //Initialize motion controller
  LittleFS.begin();   //Initialize storage

  display.begin();
  display.drawWelcomeScreen();
  delay(1000);
  
  //Initialize WiFi
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started.");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());  // ESP8266 的默认IP：192.168.4.1

  // 加载WiFi配置
  if(loadWiFiConfig()){
    WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());
    
    int retry_count = 0;  // 尝试连接WiFi，最多等待10次
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 5000)
    {
        delay(500);
        retry_count++;
        Serial.printf("Retry %d ...\n", retry_count);
    }
  }
   
  // 如果连接失败，打印状态
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi connection failed, starting captive portal...");
    handleWiFiRoute(); // 启动强制门户
  }
  else
  {
    Serial.println("WiFi connected");
    handleWiFiRoute();
    timeClient.begin();
    timeClient.update(); // 获取初始时间
  }
  delay(1000);

  button.attachClick(handleClick);
  // button.attachDoubleClick(handleDoubleClick);
  button.attachLongPressStart(handleLongPress);
  
  Serial.println("Setup finished.");
  display.clear();
  display.drawWiFiAP();
  delay(2000);
  // display.clear();
}

void loop() {
  
  button.tick();
  ledStrip.update();      //Update LED state

  batteryADC.update();  
  // 如果采样完成，更新电池电压和百分比
  if (batteryADC.isSamplingComplete()) {
    batteryVoltage = batteryADC.getVoltage();
    batteryPercentage = batteryADC.getBatteryPercentage();
    // 不再需要手动调用startNewSampling()，ADC类会根据resampleDelay自动处理
  }

  action.update();
  display.update(screenState);
  
  // Serial communication
  if (Serial.available() > 0) {  
    String cmd = Serial.readStringUntil('\n');  
    cmd.trim();  //remove CrLf

    if (cmd == "front") {  
      action.setActionState(DO_FRONT);
    } else if (cmd == "back") {  
      action.setActionState(DO_BACK);
    } else if (cmd == "left") {  
      action.setActionState(DO_LEFT);
    } else if (cmd == "right") {  
      action.setActionState(DO_RIGHT);
    } else if (cmd == "shake_left_hand") {  
      action.setActionState(DO_SHAKE_LEFT_HAND);
    } else if (cmd == "shake_right_hand") {  
      action.setActionState(DO_SHAKE_RIGHT_HAND);
    } else if (cmd == "sit_down") {  
      action.setActionState(DO_SIT_DOWN);
    } else if (cmd == "lie") {  
      action.setActionState(DO_LIE);
    } else if (cmd == "sleep") {
      action.setActionState(DO_SLEEP);
    } else if (cmd == "stand") {
      action.setActionState(DO_STAND);
    } else if (cmd == "emoji_hi") {
      screenState = EMOJI_HI;
    } else if (cmd == "emoji_happy") {
      screenState = EMOJI_HAPPY;
    } else if (cmd == "emoji_sad") {
      screenState = EMOJI_SAD;
    } else if (cmd == "emoji_angry") {
      screenState = EMOJI_ANGRY;
    } else if (cmd == "emoji_sick") {
      screenState = EMOJI_SICK;
    } else if (cmd == "emoji_love") {
      screenState = EMOJI_LOVE;
    } else if (cmd == "emoji_faint") {
      screenState = EMOJI_FAINT;
    } else if (cmd == "info_time") {
      screenState = INFO_TIME;
    } else if (cmd == "info_weather") {
      screenState = INFO_WEATHER;
    } else if (cmd == "info_power") {
      screenState = INFO_POWER;
    } else if (cmd == "oled_on") {
      ledStrip.setOn();
    } else if (cmd == "oled_off") {
      ledStrip.setOff();
    } else if (cmd == "oled_solid") {
      ledStrip.setEffect(WS2812Controller::SOLID);
    } else if (cmd == "oled_blink") {
      ledStrip.setEffect(WS2812Controller::BLINKING);
    } else if (cmd == "oled_breath") {
      ledStrip.setEffect(WS2812Controller::BREATHING);
    } else if (cmd == "oled_flow") {
      ledStrip.setEffect(WS2812Controller::FLOWING);
    } else if (cmd == "oled_rainbow") {
      ledStrip.setEffect(WS2812Controller::RAINBOW);
    } else {
      Serial.print("Unknown command: ");
    }
    Serial.println(cmd);
  }
  delay(10);
 
}
