//Control motions of the bot
#include "..\include\Action.h"


// 构造函数
ActionController::ActionController(uint8_t speed = 100) {

    currentActionState = DO_IDLE;
}

void ActionController::setActionState(Action action){

    currentActionState = action;
}

void ActionController::begin()
{
    servo_rf.attach(SERVO_PIN1, 500, 2500);
    servo_rb.attach(SERVO_PIN2, 500, 2500);
    servo_lf.attach(SERVO_PIN3, 500, 2500);
    servo_lb.attach(SERVO_PIN4, 500, 2500);
    servoSpeed = 100;

    servo_rf.write(90);                 // 旋转到 90 度
    servo_rb.write(90);                 // 旋转到 90 度
    servo_lf.write(90);                 // 旋转到 90 度
    servo_lb.write(90);                 // 旋转到 90 度
}

void ActionController::front()
{
    servo_rb.write(140); 
    servo_lf.write(40);  
    delay(100);
    servo_rf.write(40);  
    servo_lb.write(140); 
    delay(100);
    servo_rb.write(90); 
    servo_lf.write(90); 
    delay(100);
    servo_rf.write(90); 
    servo_lb.write(90); 
    delay(100);
    servo_rf.write(140);
    servo_lb.write(40); 
    delay(100);
    servo_rb.write(40); 
    servo_lf.write(140);
    delay(100);
    servo_rf.write(90); 
    servo_lb.write(90); 
    delay(100);
    servo_rb.write(90); 
    servo_lf.write(90); 
}

void ActionController::back()
{
    servo_lf.write(140);
    servo_rb.write(40); 
    delay(100);
    servo_lb.write(40); 
    servo_rf.write(140);
    delay(100);
    servo_lf.write(90); 
    servo_rb.write(90); 
    delay(100);
    servo_lb.write(90); 
    servo_rf.write(90); 
    delay(100);
    servo_lb.write(140);
    servo_rf.write(40); 
    delay(100);
    servo_lf.write(40); 
    servo_rb.write(140);
    delay(100);
    servo_lb.write(90); 
    servo_rf.write(90); 
    delay(100);
    servo_lf.write(90); 
    servo_rb.write(90); 
}

void ActionController::left()
{
    int delayTime = 100;
    int num = 0;
    while (num < 4)
    {
        servo_lf.write(80); 
        servo_rb.write(80); 
        delay(delayTime);
        servo_rf.write(120); 
        servo_lb.write(120); 
        delay(delayTime);
        servo_lf.write(40); 
        servo_rb.write(60); 
        delay(delayTime);
        servo_rf.write(140); 
        servo_lb.write(140); 
        delay(delayTime);
        servo_rf.write(90); 
        servo_lb.write(90); 
        servo_lf.write(90); 
        servo_rb.write(90); 
        delay(delayTime);
        servo_lf.write(100); 
        servo_rb.write(100); 
        delay(delayTime);
        servo_rf.write(60); 
        servo_lb.write(60); 
        delay(delayTime);
        servo_lf.write(90); 
        servo_rb.write(90); 
        delay(delayTime);
        servo_rf.write(40);
        servo_lb.write(40); 
        delay(delayTime);
        servo_rf.write(90); 
        servo_lb.write(90); 
        num++;
    }
}

void ActionController::right()
{
    int delayTime = 100;
    int num = 0;
    while (num < 4)
    {
        servo_rf.write(100); 
        servo_lb.write(100); 
        delay(delayTime);
        servo_lf.write(60); 
        servo_rb.write(60); 
        delay(delayTime);
        servo_rf.write(140); 
        servo_lb.write(140); 
        delay(delayTime);
        servo_lf.write(40); 
        servo_rb.write(40); 
        delay(delayTime);
        servo_lf.write(90); 
        servo_rb.write(90); 
            
        servo_rf.write(90); 
        servo_lb.write(90); 
        delay(delayTime);
        servo_rf.write(80); 
        servo_lb.write(80); 
        delay(delayTime);
        servo_lf.write(120); 
        servo_rb.write(120); 
        delay(delayTime);
        servo_rf.write(90); 
        servo_lb.write(90); 
        delay(delayTime);
        servo_lf.write(140);
        servo_rb.write(140); 
        delay(delayTime);
        servo_lf.write(90); 
        servo_rb.write(90); 

        num++;
    }
}

void ActionController::sitDown()
{
    servo_rf.write(90);
    servo_lf.write(90);
    servo_rb.write(170); 
    servo_lb.write(10); 
}
void ActionController::lie()
{
    servo_rf.write(180);
    servo_lf.write(0);   
    servo_rb.write(0);  
    servo_lb.write(180); 
}
void ActionController::stretch()
{
    servo_rf.write(90);
    servo_lf.write(90);
    servo_rb.write(30); 
    servo_lb.write(150); 
}

void ActionController::shakeLeftHand()
{
    servo_rf.write(90);
    servo_lf.write(90);
    servo_rb.write(150); 
    servo_lb.write(30); 
    int num = 0;
    while (num < 3)
    {
        servo_lf.write(0); 
        delay(100);
        servo_lf.write(30);
        delay(100);
        num++;
    }
    stand();
}

void ActionController::shakeRightHand()
{
    servo_rf.write(90);
    servo_lf.write(90);
    servo_rb.write(150); 
    servo_lb.write(30); 
    int num = 0;
    while (num < 3)
    {
        servo_rf.write(180);
        delay(100);
        servo_rf.write(150);
        delay(100);
        num++;
    }
    stand();
}

void ActionController::doSleep()
{
    servo_rf.write(0);                   
    servo_lf.write(180);                
    servo_rb.write(180);                
    servo_lb.write(0); 
}

void ActionController::stand(){
    servo_rf.write(90);              
    servo_lf.write(90);              
    servo_rb.write(90);               
    servo_lb.write(90); 
}

void ActionController::swingLeftRight() 
{
    int delayTime = 100;

    servo_rf.write(120);              
    servo_rb.write(120);
    delay(delayTime);      
    servo_lf.write(60);
    servo_lb.write(60);
    delay(delayTime);

    servo_rf.write(150);              
    servo_rb.write(150);               
    delay(delayTime);        
    servo_lf.write(90);
    servo_lb.write(90);
    delay(delayTime);       

    servo_rf.write(120);              
    servo_rb.write(120);
    delay(delayTime);      
    servo_lf.write(60);
    servo_lb.write(60);
    delay(delayTime);

    servo_rf.write(90);              
    servo_rb.write(90);              
    delay(delayTime); 
    servo_lf.write(30);
    servo_lb.write(30);
    delay(delayTime); 
}

void ActionController::swingFrontBack() 
{
    int delayTime = 100;

    servo_rf.write(120);              
    servo_lf.write(60);
    delay(delayTime);      
    servo_rb.write(60);               
    servo_lb.write(120);
    delay(delayTime);

    servo_rf.write(150);              
    servo_lf.write(30);
    delay(delayTime);
    servo_rb.write(90);               
    servo_lb.write(90);
    delay(delayTime);

    servo_rf.write(120);              
    servo_lf.write(60);
    delay(delayTime);      
    servo_rb.write(60);               
    servo_lb.write(120);
    delay(delayTime);
        
    servo_rf.write(90);              
    servo_lf.write(90);        
    delay(delayTime);
    servo_rb.write(30);
    servo_lb.write(150);
    delay(delayTime);
}

void ActionController::speedUp()
{
    servoSpeed++;
}

void ActionController::speedDown()
{
    servoSpeed--;
}

uint8_t ActionController::getSpeed()
{
    return servoSpeed;
}

void ActionController::update(){
    switch (currentActionState)
    {
        case DO_FRONT:          // One command one action
            front();
            currentActionState = DO_IDLE;         
            break;
        case KEEP_FRONT:    
            front();
            break;

        case DO_BACK:
            back();
            currentActionState = DO_IDLE;
            break;
        case KEEP_BACK:
            back();
            break;

        case DO_LEFT:
            left();
            currentActionState = DO_IDLE;
            break;
        case KEEP_LEFT:
            left();
            break;

        case DO_RIGHT:
            right();
            currentActionState = DO_IDLE;
            break;
        case KEEP_RIGHT:
            right();
            break;
            
        case DO_SIT_DOWN:
            sitDown();
            currentActionState = DO_IDLE;
            break;            
        case DO_LIE:
            lie();
            currentActionState = DO_IDLE;
            break;
        case DO_SHAKE_LEFT_HAND:
            shakeLeftHand();
            currentActionState = DO_IDLE;
            break;
        case DO_SHAKE_RIGHT_HAND:
            shakeRightHand();
            currentActionState = DO_IDLE;
            break;
        case DO_SLEEP:
            doSleep();
            currentActionState = DO_IDLE;
            break;
        case DO_STAND:
            stand();
            currentActionState = DO_IDLE;
            break;
        case DO_STRETCH:
            stretch();
            currentActionState = DO_IDLE;
            break;
        case DO_SWING_LEFT_RIGHT:
            swingLeftRight();
            //currentActionState = DO_IDLE;
            break;
        case DO_SWING_FRONT_BACK:
            swingFrontBack();
            //currentActionState = DO_IDLE;
            break;
        default:
            break;
    }
}
