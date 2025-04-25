//
// Class for display control
//
#include "..\include\Display.h"
#include "image.cpp"

DisplayController::DisplayController():
    u8g2(U8G2_R0, OLED_RESET_PIN, OLED_SCL_PIN, OLED_SDA_PIN)
{
    currentScreenType = SCREEN_TYPE_EMOJI;
    currentScreenState = SCREEN_NONE;
    prevScreenState = SCREEN_NONE;
    
    // Setting title display
    settingTitleStartTime = 0;
    showingSettingTitle = false;
    currentSettingTitle = "";
}

void DisplayController::begin()
{
    u8g2.begin();
    u8g2.setDisplayRotation(U8G2_R2);
}

void DisplayController::clear(){
    u8g2.clearDisplay();
}

void DisplayController::drawWelcomeScreen()
{
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.firstPage();
    do{
        u8g2.drawXBMP(0, 15, 56, 33, aws_logo_56x33);
        u8g2.drawStr(90, 20, "GCR");
        u8g2.drawStr(75, 35, "INDUSTRY");   //70
        u8g2.drawStr(75, 50, "BUILDERS");
    } while (u8g2.nextPage());

    //黄蓝是屏上 1/4 部分为黄光,下3/4为蓝
    // u8g2.firstPage();
    // do{
    //   u8g2.drawXBMP(0, 0, 128, 64, aws_logo_128x64);
    // } while (u8g2.nextPage());
    // delay(1000);
}

void DisplayController::drawWiFiAP(){
    do{
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawXBMP(0, 9, 56, 46, wifi_56x46);
        u8g2.drawStr(70, 20, "WIFI_AP");
        u8g2.drawStr(70, 35, "PandaBot");
        u8g2.drawStr(70, 50, "192.168.4.1");
    } while (u8g2.nextPage());
}

void DisplayController::drawWiFiNotConnected(){
    do{
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawXBMP(0, 9, 56, 46, wifi_56x46);
        u8g2.drawStr(70, 20, "IP:");
        u8g2.drawStr(70, 35, "192.168.4.1");
        u8g2.drawStr(70, 50, "Need Network");
    } while (u8g2.nextPage());
}

void DisplayController::drawTime(){
    do{
        timeClient.update();    // 更新时间
        int currentHour = timeClient.getHours();
        int currentMinute = timeClient.getMinutes();
        String timeToDisplay = String(currentHour) + ":" + String(currentMinute);

        u8g2.setFont(u8g2_font_ncenB14_tr);
        u8g2.drawXBMP(0, 4, 56, 56, time_56x56);
        u8g2.drawStr(75, 30, "TIME");
        u8g2.setCursor(75, 50);
        u8g2.print(timeToDisplay);
        } while (u8g2.nextPage());
    }

void DisplayController::drawWeather(){
    if (weather == "阴" || weather == "多云")
    {
        do{
            u8g2.setFont(u8g2_font_ncenB08_tr);
            u8g2.drawXBMP(0, 10, 56, 43, cloudy_56x43);
            u8g2.drawStr(75, 15, "Temp");
            String temperatureString = String(temperature) + " C";
            u8g2.drawStr(75, 25, temperatureString.c_str());
            u8g2.drawStr(75, 45, "Humidity");
            String humidityString = String(humidity) + " %";
            u8g2.drawStr(75, 55, humidityString.c_str());
        } while (u8g2.nextPage());

        if (weather == "多云") Serial.println("weather_cloudy");
        else Serial.println("weather_overcast");
    }
    else if (weather == "小雨" || weather == "大雨" || weather == "暴雨" || weather == "雨")
    {
        do{
            u8g2.setFont(u8g2_font_ncenB08_tr);
            u8g2.drawXBMP(0, 3, 56, 58, rainy_56x58);
            u8g2.drawStr(75, 15, "Temp");
            String temperatureString = String(temperature) + " C";
            u8g2.drawStr(75, 25, temperatureString.c_str());
            u8g2.drawStr(75, 45, "Humidity");
            String humidityString = String(humidity) + " %";
            u8g2.drawStr(75, 55, humidityString.c_str());
        } while (u8g2.nextPage());
        Serial.println("weather_rainy");
    }
    else if (weather == "晴")
    {
        do{
            u8g2.setFont(u8g2_font_ncenB08_tr);
            u8g2.drawXBMP(0, 4, 56, 56, sunny_56x56);
            u8g2.drawStr(75, 15, "Temp");
            String temperatureString = String(temperature) + " C";
            u8g2.drawStr(75, 25, temperatureString.c_str());
            u8g2.drawStr(75, 45, "Humidity");
            String humidityString = String(humidity) + " %";
            u8g2.drawStr(75, 55, humidityString.c_str());
        } while (u8g2.nextPage());
        Serial.println("weather_sunny");
    }
}

void DisplayController::drawPowerStatus(){
    do{
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawXBMP(0, 4, 56, 56, battery_56x56);
        u8g2.drawStr(75, 20, "BATTERY");
        u8g2.setFont(u8g2_font_ncenB14_tr);
        char str[8]; 
        snprintf(str, sizeof(str), "%.1f V", batteryVoltage);
        u8g2.drawStr(75, 40, str);
        snprintf(str, sizeof(str), "%d %%", batteryPercentage);
        u8g2.drawStr(75, 60, str);
    } while (u8g2.nextPage());

}

void DisplayController::setScreenType(ScreenType screenType){
  currentScreenType = screenType;
}

void DisplayController::setScreenState(Screen screenState){
  currentScreenState = screenState;
}

void DisplayController::drawSettingTitle(const String& title)
{
    u8g2.clearDisplay();
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.firstPage();
    do {
        u8g2.drawXBMP(0, 4, 56, 56, gear_56x56);
        int titleWidth = u8g2.getStrWidth(title.c_str());
        int x = 68 + (64 - titleWidth) / 2; // 右侧区域的中心点
        u8g2.drawStr(x, 36, title.c_str());
    } while (u8g2.nextPage());
}

void DisplayController::showSettingTitle(const String& title)
{
    prevScreenState = currentScreenState;
    showingSettingTitle = true;
    currentSettingTitle = title;
    settingTitleStartTime = millis();    
    
    drawSettingTitle(title);
}

// 无条件重绘制屏幕
void DisplayController::drawScreen(Screen screenState)
{
  u8g2.clearDisplay();         
  switch (screenState)
  {
    case EMOJI_HI:
      do{
          u8g2.drawXBMP(0, 0, 128, 64, hi);
      } while (u8g2.nextPage());
      break;

    case EMOJI_HAPPY:
      do{
        u8g2.drawXBMP(0, 0, 128, 64, happy);
      } while (u8g2.nextPage());
      break;

    case EMOJI_ANGRY:
      do{
          u8g2.drawXBMP(0, 0, 128, 64, angry);
      } while (u8g2.nextPage());
      break;

    case EMOJI_SAD:
      do{
          u8g2.drawXBMP(0, 0, 128, 64, sad);
      } while (u8g2.nextPage());
      break;

    case EMOJI_LOVE: 
      do{
          u8g2.drawXBMP(0, 0, 128, 64, love);
      } while (u8g2.nextPage());
      break;

    case EMOJI_SICK: 
      do
        u8g2.drawXBMP(0, 0, 128, 64, sick);
      while (u8g2.nextPage());
      break;

    case EMOJI_FAINT:
      do
          u8g2.drawXBMP(0, 0, 128, 64, faint);
      while (u8g2.nextPage());
      break;

    case EMOJI_BLINK:
      break;

    case INFO_WEATHER:
      if (WiFi.status() != WL_CONNECTED)
      {
          drawWiFiNotConnected();
      }
      else
      {
          fetchWeather();
          drawWeather();
      }
      break;

    case INFO_TIME:
      if (WiFi.status() != WL_CONNECTED)
          drawWiFiNotConnected();
      else
          drawTime();
      break;
      
    case INFO_POWER:
      drawPowerStatus();
      break;
        
    default:
        break;
    }
}

//根据状态绘制屏幕
void DisplayController::update(Screen screenState)
{
  // 检查是否在显示设置标题，如果是且超过了显示时间，则恢复之前的显示内容
  if (showingSettingTitle && (millis() - settingTitleStartTime >= SETTING_TITLE_DISPLAY_TIME)) {
    showingSettingTitle = false;
    currentScreenState = prevScreenState; // 恢复到之前的状态
    drawScreen(currentScreenState);
  }

  // 如果正在显示设置标题，则不更新屏幕
  if (showingSettingTitle) return;

  // 如果状态没有变化，则不更新屏幕
  if (screenState == currentScreenState) return;
  currentScreenState = screenState; // 更新状态
  drawScreen(currentScreenState);
}

