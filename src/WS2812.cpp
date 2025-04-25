// WS2812.cpp
#include "../include/WS2812.h"

// 初始化LED灯带
void WS2812Controller::begin() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  FastLED.clear();
  FastLED.show();
}

// 开关灯
void WS2812Controller::toggleOnOff() {
  isOn = !isOn;
  if (!isOn) {
    FastLED.clear();
    FastLED.show();
  } else {
    // 恢复之前的效果
    (this->*currentEffect)(currentColor);
  }
}

void WS2812Controller::setOn() {
    isOn = true;
    (this->*currentEffect)(currentColor);
}

void WS2812Controller::setOff() {
    isOn = false;
    // FastLED.clear();
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;  // 设置为黑色
    }
    FastLED.show();
}

// 设置效果
void WS2812Controller::setEffect(Effect effect) {
  if (!isOn) {
    isOn = true;  // 设置效果时自动开灯
  }
  
  currentEffectIndex = effect;
  
  // 重置效果参数
  flowingPos = 0;
  breathingValue = 0;
  breathingUp = true;
  blinkState = false;
  
  // 设置对应的效果函数
  switch (effect) {
    case SOLID:
      currentEffect = &WS2812Controller::solidColor;
      break;
    case FLOWING:
      currentEffect = &WS2812Controller::flowingEffect;
      break;
    case BREATHING:
      currentEffect = &WS2812Controller::breathingEffect;
      break;
    case RAINBOW:
      currentEffect = &WS2812Controller::rainbowEffect;
      break;
    case BLINKING:
      currentEffect = &WS2812Controller::blinkEffect;
      break;
    default:
      currentEffect = &WS2812Controller::solidColor;
      break;
  }
  
  // 立即应用效果
  (this->*currentEffect)(currentColor);
}

// 设置亮度
void WS2812Controller::setBrightness(uint8_t bright) {
  brightness = constrain(bright, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  FastLED.setBrightness(brightness);
  FastLED.show();
}

// 增加亮度
void WS2812Controller::increaseBrightness(uint8_t amount) {
  uint16_t newBrightness = brightness + amount;
  setBrightness(newBrightness > MAX_BRIGHTNESS ? MAX_BRIGHTNESS : newBrightness);
}

// 减少亮度
void WS2812Controller::decreaseBrightness(uint8_t amount) {
  uint16_t newBrightness = brightness - amount;
  setBrightness(newBrightness < MIN_BRIGHTNESS ? MIN_BRIGHTNESS : newBrightness);
}

// 更新函数 - 在loop中调用
void WS2812Controller::update() {
  if (!isOn) return;
  
  unsigned long currentTime = millis();
  
  // 根据不同效果设置不同的更新频率
  unsigned long updateInterval;
  switch (currentEffectIndex) {
    case FLOWING:
      updateInterval = 100; // 流水灯更新间隔
      break;
    case BREATHING:
      updateInterval = 20;  // 呼吸灯更新间隔
      break;
    case RAINBOW:
      updateInterval = 20;  // 彩虹效果更新间隔
      break;
    case BLINKING:
      updateInterval = 200; // 闪烁效果更新间隔
      break;
    default:
      updateInterval = 100;
      break;
  }
  
  // 检查是否需要更新
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
    (this->*currentEffect)(currentColor); // 调用当前效果函数
  }
}

// 纯色效果
void WS2812Controller::solidColor(CRGB color) {
  CRGB useColor = color.r == 0 && color.g == 0 && color.b == 0 ? currentColor : color;
  
  useColor.maximizeBrightness();
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = useColor;
  }
  FastLED.show();
}

// 流水灯效果
void WS2812Controller::flowingEffect(CRGB color) {
  FastLED.clear();
  
  // 为每个组设置流水灯效果
  int baseIndex = 0;
  int pos = (flowingPos + baseIndex) % LEDS_PER_GROUP;
    
  // 设置主LED
  currentColor.maximizeBrightness();
  leds[baseIndex + pos] = currentColor;
  leds[NUM_LEDS-1 - (baseIndex + pos)] = leds[baseIndex + pos];

  // 设置尾部渐变效果
  int tailPos1 = (pos - 1 + LEDS_PER_GROUP) % LEDS_PER_GROUP;
  int tailPos2 = (pos - 2 + LEDS_PER_GROUP) % LEDS_PER_GROUP;
  int tailPos3 = (pos - 3 + LEDS_PER_GROUP) % LEDS_PER_GROUP;
    
  leds[baseIndex + tailPos1] = currentColor.nscale8(192); // 75%亮度
  leds[NUM_LEDS - 1 - (baseIndex + tailPos1)] = leds[baseIndex + tailPos1];
  leds[baseIndex + tailPos2] = currentColor.nscale8(128);  // 50%亮度
  leds[NUM_LEDS - 1 - (baseIndex + tailPos2)] = leds[baseIndex + tailPos2];
  leds[baseIndex + tailPos3] = currentColor.nscale8(64);  // 25%亮度
  leds[NUM_LEDS - 1 - (baseIndex + tailPos3)] = leds[baseIndex + tailPos3];
  
  // 更新位置
  flowingPos = (flowingPos + 1) % LEDS_PER_GROUP;

  FastLED.show();
}

// 呼吸灯效果
void WS2812Controller::breathingEffect(CRGB color) {
  // 计算呼吸效果的亮度值（使用正弦波）
  float breath = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
  uint8_t breathBrightness = breath;
  
  // 应用到所有LED
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = currentColor;
  }
  
  // 设置呼吸亮度
  FastLED.setBrightness(breathBrightness);
  FastLED.show();
  
  // 恢复原始亮度设置（仅用于显示）
  FastLED.setBrightness(brightness);
}

// 彩虹效果
void WS2812Controller::rainbowEffect(CRGB color) {
  // 为每个组设置彩虹效果
  // for (int group = 0; group < NUM_GROUPS; group++) {
  //   int baseIndex = group * LEDS_PER_GROUP;
    
  //   for (int i = 0; i < LEDS_PER_GROUP; i++) {
  //     // 使用HSV色彩空间创建彩虹效果
  //     // 每个LED的色调值略有不同，形成彩虹
  //     leds[baseIndex + i] = CHSV(
  //       rainbowHue + (i * 256 / LEDS_PER_GROUP),
  //       255, // 饱和度
  //       255  // 亮度
  //     );
  //   }
  // }
    
  int baseIndex = 0;
  for (int i = 0; i < LEDS_PER_GROUP; i++) {
    // 使用HSV色彩空间创建彩虹效果
    // 每个LED的色调值略有不同，形成彩虹
    leds[baseIndex + i] = CHSV(
      rainbowHue + (i * 256 / LEDS_PER_GROUP),
      255, // 饱和度
      255  // 亮度
    );
  }

  baseIndex = NUM_LEDS-1;
  for (int i = 0; i < LEDS_PER_GROUP; i++) {
    leds[baseIndex - i] = CHSV(
      rainbowHue + (i * 256 / LEDS_PER_GROUP),
      255, // 饱和度
      255  // 亮度
    );
  }
   
  // 更新彩虹色调
  rainbowHue++;
  FastLED.show();
}

// 闪烁效果
void WS2812Controller::blinkEffect(CRGB color) {
  FastLED.setBrightness(255);
  if (blinkState) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = currentColor;   // 点亮所有LED
    }
  } else {
    FastLED.clear();  // 熄灭所有LED
  }
  
  blinkState = !blinkState;   // 切换状态
  FastLED.show();
}

// 辅助函数：应用函数到所有组
void WS2812Controller::applyToAllGroups(void (WS2812Controller::*func)(int)) {
  for (int group = 0; group < NUM_GROUPS; group++) {
    (this->*func)(group);
  }
}

// 辅助函数：设置组内像素
void WS2812Controller::setGroupPixel(int pos, CRGB color) {
  for (int group = 0; group < NUM_GROUPS; group++) {
    leds[group * LEDS_PER_GROUP + pos] = color;
  }
}
