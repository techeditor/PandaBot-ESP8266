// Action.h
#ifndef ACTION_H
#define ACTION_H

#include <Servo.h>
#include "main.h"

enum Action {
    DO_IDLE,
    DO_STAND,
    DO_FRONT,
    DO_BACK,
    DO_LEFT,
    DO_RIGHT,
    KEEP_FRONT,
    KEEP_BACK,
    KEEP_LEFT,
    KEEP_RIGHT,
    DO_SIT_DOWN,
    DO_LIE,
    DO_SHAKE_LEFT_HAND,
    DO_SHAKE_RIGHT_HAND,
    DO_SLEEP,
    DO_STRETCH,
    DO_SWING_LEFT_RIGHT,
    DO_SWING_FRONT_BACK,
    ACTION_COUNT
};

class ActionController {
    public:
        // 构造函数
        ActionController(uint8_t speed);

        void setActionState(Action action);
        Action getScreenState() {return currentActionState;}
        void update();
        void begin();
        
        void stand();
        void front();
        void back();
        void left();
        void right();
        void sitDown();
        void lie();
        void shakeLeftHand();
        void shakeRightHand();
        void doSleep();
        void stretch();
        void swingLeftRight();
        void swingFrontBack();
        void speedUp();
        void speedDown();
        uint8_t getSpeed();

    private:
        Servo servo_lf;     //front left
        Servo servo_lb;     //back left
        Servo servo_rf;     //front right
        Servo servo_rb;     //back right
        
        int servo_lf_offset_min;
        int servo_lf_offset_max;
        int servo_lb_offset_min;
        int servo_lb_offset_max;
        int servo_rf_offset_min;
        int servo_rf_offset_max;
        int servo_rb_offset_min;
        int servo_rb_offset_max;

        int servoSpeed;     // 舵机转速
        Action currentActionState;
};

#endif