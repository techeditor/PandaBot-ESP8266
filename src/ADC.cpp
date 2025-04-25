#include "..\include\ADC.h"

// 构造函数
ADC::ADC(int pin) {
    adcPin = pin;
    totalAdcValue = 0;
    sampleCount = 0;
    lastSampleTime = 0;
    lastCompleteSampleTime = 0;
    averageVoltage = 0.0;
    batteryPercentage = 0;
    samplingComplete = false;
    readyForNewSample = true;
}

// 初始化
void ADC::begin() {
    // 在ESP32/ESP8266上，通常不需要特别的ADC初始化
    // 如果有特殊需求，可以在这里添加
    startNewSampling();
}

// 更新ADC读数（非阻塞）
void ADC::update() {
    unsigned long currentTime = millis();
    
    // 如果采样已完成，检查是否到达重新采样的时间
    if (samplingComplete) {
        if (currentTime - lastCompleteSampleTime >= this->resampleDelay) {
            readyForNewSample = true;
        } else {
            return;  // 等待重新采样时间
        }
    }
    
    // 如果准备好开始新一轮采样
    if (readyForNewSample) {
        startNewSampling();
        readyForNewSample = false;
        return;
    }
    
    // 检查是否到达采样间隔时间
    if (currentTime - lastSampleTime >= this->sampleInterval) {
        
        lastSampleTime = currentTime;
        totalAdcValue += analogRead(adcPin);    // 读取ADC值并累加
        sampleCount++;
        
        // 检查是否完成所有采样
        if (sampleCount >= numSamples) {
            float averageAdcValue = totalAdcValue / (float)numSamples;  // 计算平均ADC值
            averageVoltage = (averageAdcValue / 1023.0) * 1.0;  // 将ADC值转换为电压（ESP8266的参考电压为1.0V）
            batteryPercentage = mapBatteryPercentage(averageVoltage * voltageDividerRatio);
            
            samplingComplete = true;    // 标记采样完成并记录完成时间
            lastCompleteSampleTime = currentTime;
        }
    }
}

// 获取当前电压值
float ADC::getVoltage() {
    return averageVoltage * voltageDividerRatio;
}

// 获取电池百分比
int ADC::getBatteryPercentage() {
    return batteryPercentage;
}

// 检查采样是否完成
bool ADC::isSamplingComplete() {
    return samplingComplete;
}

// 开始新一轮采样
void ADC::startNewSampling() {
    totalAdcValue = 0;
    sampleCount = 0;
    lastSampleTime = millis();
    samplingComplete = false;
}

// 计算电池电量百分比的函数（私有辅助方法）
int ADC::mapBatteryPercentage(float voltage) {
    if (voltage <= this->minVoltage)
        return 0;           // 小于等于最小电压时，电量为0%
    if (voltage >= this->maxVoltage)
        return 100;         // 大于等于最大电压时，电量为100%

    // 根据线性比例计算电量百分比
    return (int)((voltage - this->minVoltage) / (this->maxVoltage - this->minVoltage) * 100);
}
