// ADC.h
#ifndef ADC_H
#define ADC_H

#include "main.h"

class ADC {
private:
    const float voltageDividerRatio = 8.4;  // 分压比（8.4倍缩小）
    const float minVoltage = 6.4;           // 电压为0%时
    const float maxVoltage = 8.4;           // 电压为100%时
    const int numSamples = 10;              // 采样数量
    const unsigned long sampleInterval = 10;  // 每次采样间隔时间（毫秒）
    const unsigned long resampleDelay = 1000; // 两次完整采样之间的间隔（毫秒）
    
    int adcPin;                             // ADC引脚
    long totalAdcValue;                     // 采样总值
    int sampleCount;                        // 当前采样计数
    unsigned long lastSampleTime;           // 上次采样时间
    unsigned long lastCompleteSampleTime;   // 上次完成采样的时间
    float averageVoltage;                   // 平均电压值
    int batteryPercentage;                  // 电池百分比
    bool samplingComplete;                  // 采样完成标志
    bool readyForNewSample;                 // 是否准备好开始新一轮采样
    
    int mapBatteryPercentage(float voltage); // 计算电池电量百分比
    
public:
    ADC(int pin = A0);                      // 构造函数，默认使用A0引脚
    
    void begin();                           // 初始化
    void update();                          // 更新ADC读数（非阻塞）
    float getVoltage();                     // 获取当前电压值
    int getBatteryPercentage();             // 获取电池百分比
    bool isSamplingComplete();              // 检查采样是否完成
    void startNewSampling();                // 开始新一轮采样
};

#endif
