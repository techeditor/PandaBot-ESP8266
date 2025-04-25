//display.h

#ifndef DISPLAY_H
#define DISPLAY_H

#include "main.h"

#define SETTING_TITLE_DISPLAY_TIME 2000  // 设置标题显示时间（毫秒）
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

enum Screen{
    SCREEN_NONE,
    EMOJI_HI,
    EMOJI_HAPPY,
    EMOJI_SAD,
    EMOJI_ANGRY,
    EMOJI_LOVE,
    EMOJI_FAINT,
    EMOJI_SICK,
    EMOJI_BLINK,
    INFO_WELCOME = 100,
    INFO_WIFI_AP,
    INFO_WIFI_NOT_CONNECTED,
    INFO_TIME,
    INFO_WEATHER,
    INFO_POWER,
    INFO_SETTING_ITEM,
    SCREEN_COUNT
};

enum ScreenType{
    SCREEN_TYPE_EMOJI,
    SCREEN_TYPE_INFO
};

class DisplayController {
    public:
        DisplayController();    // 构造函数
        void begin();
        void clear();

        void setScreenType(ScreenType screenType);
        ScreenType getScreenType() {return currentScreenType;}
        void setScreenState(Screen screenState);
        Screen getScreenState() {return currentScreenState;}
        void drawScreen(Screen screenState);
        void update(Screen screenState);
        
        void drawWelcomeScreen();
        void drawWiFiAP();
        void drawWiFiNotConnected();
        void drawTime();
        void drawWeather();
        void drawPowerStatus();
        void drawSettingTitle(const String& title);  // 绘制设置标题和齿轮图标
        void showSettingTitle(const String& title);  // 显示设置标题并设置相关状态

        // void draw_eyes(bool update = true);
        // void saccade(int direction_x, int direction_y);
        // void move_eye(int direction);
        // void eye_center(bool update = true);
        // void eye_blink(int speed = 12);
        // void eye_sleep();
        // void eye_wakeup();
        // void eye_happy();
        // void eye_sad();
        // void eye_anger();
        // void eye_surprise();
        // void eye_right();
        // void eye_left();
        // void print_angle();

        U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2;

    private:
        ScreenType currentScreenType;
        Screen currentScreenState;
        Screen prevScreenState;
        
        // Setting title display
        unsigned long settingTitleStartTime;  // 设置标题显示开始时间
        bool showingSettingTitle;             // 是否正在显示设置标题
        String currentSettingTitle;           // 当前显示的设置标题
        
        // Adjustable
        int ref_eye_height;
        int ref_eye_width;
        int ref_space_between_eye;
        int ref_corner_radius;

        // Current state of the eyes
        int left_eye_height;
        int left_eye_width;
        int left_eye_x;
        int left_eye_y;

        int right_eye_height;
        int right_eye_width;
        int right_eye_x;
        int right_eye_y;
};

#endif
