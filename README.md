# PandaBot

<div align="center">
  <img src="https://img.shields.io/badge/平台-ESP8266-blue.svg" alt="平台">
  <img src="https://img.shields.io/badge/框架-Arduino-green.svg" alt="框架">
  <img src="https://img.shields.io/badge/开发工具-PlatformIO-orange.svg" alt="开发工具">
</div>

## 项目简介

PandaBot是一个基于ESP8266的四足机器人项目，根据[嘉立创的“EDA-Robot机器狗项目”](https://wiki.lceda.cn/zh-hans/course-projects/smart-internet/eda-robot/eda-robot-introduce.html) (以下简称原项目) 改造而来。

PandaBot具有多种动作控制、表情显示、LED灯效和Web远程控制功能。机器人通过四个舵机控制四肢运动，使用OLED显示屏展示表情和信息，并配备WS2812 LED灯带提供丰富的灯光效果。

项目提供了直观的Web界面，可以通过手机或电脑远程控制机器人的动作、表情和LED灯效，还可以查看电池电量、时间和天气信息。

本项目开源内容包括三部分：
* 硬件：请参考立创开源硬件平台的[项目](https://oshwhub.com/techeditor/works)。
* 软件：请参考本GitHub项目。
* 3D打印：请参考拓竹Maker World的[项目](https://makerworld.com.cn/zh/@techeditor)。


<img src="doc\images\PandaBot.jpg" width="800px" alt="PandaBot">

## 与原项目的区别
本项目主要在以下方面对原项目进行了改进和创新：
* 增加了更多外设，包括触摸按钮、LED灯带、语音控制芯片，充分利用了ESP8266的IO资源。
* 重构了程序。原项目中只有一个单独程序文件```main.cpp```，本项目将其中的相关逻辑抽象成不同模块的类，便于代码的开发和维护。
* 将SPIFF替换为LittleFS库，将DynamicJsonDocument替换为JsonDocument库，新增OneButton库。
* 非阻塞处理。将原程序中部分的```delay()```改造为非阻塞处理方式，例如ADC采样，以支持更多组件和更复杂的处理逻辑。
* 重新设计了Web界面，样式更活泼，并且支持对LED的控制，对LittleFS文件系统的异步读写。
* 设计了全新的Minecraft风格熊猫外壳。  

## 功能特点

### 动作控制
- 基本移动：前进、后退、左转、右转。
- 姿态动作：站立、坐下、趴下、睡觉、伸懒腰。
- 特殊动作：摇左手、摇右手、左右摇摆、前后摇摆。

### 表情显示
- 多种表情：问候、开心、悲伤、生气、爱心、晕倒、生病等。
- 信息显示：时间、天气、电池电量。

### LED灯效
- 多种灯效：常亮、闪烁、呼吸、流水、彩虹。
- 自定义颜色：支持RGB颜色选择。

### 控制方式
- Web界面控制：通过手机或电脑远程控制。
- 触摸按钮控制：通过机身触摸按钮控制。
- 语音控制：通过ASRPRO语音芯片识别命令词，再通过串口通信控制。

### 其他功能
- WiFi连接：支持连接到现有WiFi网络。
- 天气查询：支持查询当前天气信息。
- 电池监测：实时监测电池电量和电压。

## 主要硬件要求

- ESP8266 (ESP-12F) 模块。
- 4个SG90舵机
- SSD1306 OLED显示屏 (128x64分辨率)
- WS2812 LED灯带 (12个LED，分为2组)
- TTP223 电容式触摸传感器模块 (触摸按钮)
- ASRPRO 核心板
- 14500 3.7V 锂电池
- 2串/3串锂电池组充电模块

## 引脚定义

|功能|引脚|
|----|----|
| 舵机 | 前右：GPIO14 <br> 后右：GPIO16 <br> 前左：GPIO12 <br> 后左：GPIO13
| OLED显示屏 | SDA：GPIO4 <br> SCL：GPIO5
| WS2812 LED灯带 | SIG：GPIO2
| 触摸按钮 | SIG：GPIO15
| 电压监测 | ADC引脚：A0

## 软件依赖

项目依赖以下库：

- [FastLED](https://github.com/FastLED/FastLED)：用于控制WS2812 LED灯带
- [U8g2](https://github.com/olikraus/u8g2)：用于控制OLED显示屏
- [ArduinoJson](https://arduinojson.org/)：用于JSON数据处理
- [NTPClient](https://github.com/arduino-libraries/NTPClient)：用于获取网络时间
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)：用于异步Web服务器
- [OneButton](https://github.com/mathertel/OneButton)：用于按钮事件处理

## 安装和配置

### 开发环境设置

1. 安装[PlatformIO](https://platformio.org/)（推荐）或Arduino IDE
2. 克隆或下载本项目代码
3. 使用PlatformIO打开项目文件夹

### 依赖库安装

使用PlatformIO，依赖库会自动安装。如果使用Arduino IDE，需要手动安装上述依赖库。

### WiFi和天气API配置

1. 复制`data/ssid.json.example`为`data/ssid.json`
2. 编辑`ssid.json`文件，填入以下信息：
   ```json
   {
       "ssid": "你的WiFi名称",
       "password": "你的WiFi密码",
       "city": "你的城市拼音（如beijing）",
       "key": "你的心知天气API密钥"
   }
   ```
3. 获取[心知天气API密钥](https://www.seniverse.com/)（可选，用于天气功能）

### 编译和上传

推荐使用PlatformIO的命令菜单进行编译、上传、监视串口、构建文件系统、上传文件系统。

以下是简单的pio命令。

1. 编译项目：
   ```
   pio run
   ```

2. 上传固件：
   ```
   pio run --target upload
   ```

3. 上传文件系统（包含Web界面和配置文件）：
   ```
   pio run --target uploadfs
   ```

## 使用说明

### 首次启动

1. 上电后，PandaBot会自动创建一个名为"PandaBot"的WiFi接入点（无密码）。
2. 使用手机或电脑连接到该WiFi。
3. 打开浏览器，访问`192.168.4.1`进入Web控制界面。
4. 在"配置"页面中设置WiFi连接信息和天气API密钥（可选）。

### Web界面操作

- **主页**：提供"遥控器"和"配置"两个入口
- **遥控器页面**：
  - 运动控制：控制机器人的各种动作
  - 表情控制：切换不同的表情
  - 信息服务：显示时间和天气信息
  - LED控制：设置LED灯带的效果和颜色
<img src="doc\images\control.jpg" width="300px" alt="control">

- **配置页面**：
  - 设置WiFi连接信息
  - 设置天气API密钥和城市
<img src="doc\images\setting.jpg" width="300px" alt="setting">


### 按钮操作

- **长按**：切换设置模式（动作、表情、信息、LED）
- **单击**：根据当前设置模式执行不同操作
  - 动作模式：切换动作
  - 表情模式：切换表情
  - 信息模式：切换信息显示
  - LED模式：切换LED效果


### ASRPRO 语音命令

- 动作命令：前进、向前、走啊、后退、向后、趴下、坐着、坐下、休息、睡觉、抬左腿、抬左手、抬右腿、抬右手、停下、停止、别动了、别走了。
- 表情命令：高兴、开心、悲伤、难过、不高兴、不开心、生气、愤怒、喜欢、晕、恶心、不舒服。
- 信息命令：显示时间、显示天气、显示电量
- LED命令：关灯、开灯、常亮、闪烁、呼吸灯、流水灯、彩虹灯

如需要修改命令，请使用[“天文Block”](http://www.twen51.com/new/twen51/art_show.php?id=230)软件编辑 asrpro/PandaBot_xxxxxxxx.hd 文件。


## 项目结构

```
PandaBot/
├── data/                  # Web界面和配置文件
│   ├── control.html       # 控制页面
│   ├── index.html         # 主页
│   ├── setting.html       # 设置页面
│   └── ssid.json.example  # WiFi配置示例
├── include/               # 头文件
│   ├── Action.h           # 动作控制
│   ├── ADC.h              # 电池监测
│   ├── Display.h          # 显示控制
│   ├── main.h             # 主头文件
│   ├── Storage.h          # 存储功能
│   ├── Weather.h          # 天气功能
│   └── WS2812.h           # LED控制
├── lib/                   # 库文件
├── asrpro/                # ASRPRO 程序
├── src/                   # 源代码
│   ├── Action.cpp         # 动作控制实现
│   ├── ADC.cpp            # 电池监测实现
│   ├── Display.cpp        # 显示控制实现
│   ├── image.cpp          # 图像资源
│   ├── main.cpp           # 主程序
│   ├── Storage.cpp        # 存储功能实现
│   ├── Weather.cpp        # 天气功能实现
│   └── WS2812.cpp         # LED控制实现
├── platformio.ini         # PlatformIO配置
└── README.md              # 项目说明
```

## 待改进的地方
本项目在以下方面有待进一步改进：
* 将```Action.cpp```变为非阻塞处理。
* 使用RTOS重构程序。
* 显示动画表情。
  

<!-- ## 贡献指南

欢迎为PandaBot项目做出贡献！您可以通过以下方式参与：

1. 报告问题或提出功能建议
2. 提交代码改进或新功能
3. 改进文档或添加示例

## 许可证

本项目采用MIT许可证。 -->


---

© 2025 PandaBot by Techeditor
