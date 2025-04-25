// WS2812.h
#ifndef WS2812_H
#define WS2812_H

#include <FastLED.h>

#define DATA_PIN  2     // GPIO2 of ESP8266

#define NUM_LEDS 12       // LED总数
#define LEDS_PER_GROUP 6  // 每组LED数量
#define NUM_GROUPS 2      // LED组数
#define BRIGHTNESS 100    // 默认亮度
#define MAX_BRIGHTNESS 255 // 最大亮度
#define MIN_BRIGHTNESS 20  // 最小亮度

class WS2812Controller {
public:
  // 效果枚举
  enum Effect {
    LED_OFF,
    SOLID,     // 纯色
    FLOWING,   // 流水灯
    BREATHING, // 呼吸灯
    RAINBOW,   // 彩虹效果
    BLINKING,  // 闪烁效果
    LED_EFFECT_COUNT,
  };

  // 构造函数
  WS2812Controller() :
    isOn(false), 
    brightness(BRIGHTNESS), 
    currentEffectIndex(SOLID),
    currentColor(CRGB::White),
    flowingPos(0),
    breathingValue(0),
    breathingUp(true),
    rainbowHue(0),
    blinkState(false),
    lastUpdateTime(0)
  {
    // 初始化效果函数指针
    currentEffect = &WS2812Controller::solidColor;
  }
  
  // 初始化LED灯带
  void begin();
  
  // 开关控制
  void toggleOnOff();
  void setOn();
  void setOff();
  bool getState() { return isOn; }
    
  // 效果设置
  void setEffect(Effect effect);
  Effect getCurrentEffect() { return currentEffectIndex; }
  
  // 颜色设置
  void setColor(CRGB color) { currentColor = color; }
  CRGB getColor() { return currentColor; }
  
  // 亮度控制
  void setBrightness(uint8_t bright);
  void increaseBrightness(uint8_t amount = 10);
  void decreaseBrightness(uint8_t amount = 10);
  uint8_t getBrightness() { return brightness; }
  
  // 更新函数 - 在loop中调用
  void update();
  
  // 效果函数
  void solidColor(CRGB color = CRGB::White);
  void flowingEffect(CRGB color);
  void breathingEffect(CRGB color);
  void rainbowEffect(CRGB color);
  void blinkEffect(CRGB color);

private:
  int dataPin;    // data pin
  CRGB leds[NUM_LEDS];  // LED数组
  bool isOn;            // 开关状态
  uint8_t brightness;   // 亮度值
  Effect currentEffectIndex; // 当前效果索引
  CRGB currentColor;    // 当前颜色
  
  // 效果参数
  uint8_t flowingPos;   // 流水灯位置
  uint8_t breathingValue; // 呼吸灯值
  bool breathingUp;     // 呼吸灯方向
  uint8_t rainbowHue;   // 彩虹色调
  bool blinkState;      // 闪烁状态
  unsigned long lastUpdateTime; // 上次更新时间
  
  // 效果函数指针
  typedef void (WS2812Controller::*EffectFunction)(CRGB color);
  EffectFunction currentEffect;
  
  // 辅助函数
  void applyToAllGroups(void (WS2812Controller::*func)(int));
  void setGroupPixel(int pos, CRGB color);
};

#endif
