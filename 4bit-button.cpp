/*
 * SevSegFour
 * 驱动四位数码管显示0到9999
 */
#include "SevSeg.h"

SevSeg sevseg;

byte numDigits = 4;                              //数码管位数
byte digitPins[] = {2, 3, 12, 13};               //数码管公共极连接Arduino的引脚
byte segmentPins[] = {7, 6, 5, 10, 11, 8, 9, 4}; //数码管a,b,c,d,e,f,g,dp对应引脚
byte hardwareConfig = COMMON_CATHODE;            // 共阴极数码管

int group = 1;
int lastRM = 10;
int RM = 10;
int delaycount = 0;
int trainTime = 0;
bool minusBtnReleased = true;
bool plusBtnReleased = true;
bool confirmBtnReleased = true;
bool training = false;
bool sceneChanged = false;

void setup()
{
    Serial.begin(9600);
    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins); //初始化数码管
    pinMode(A0, INPUT);
    digitalWrite(A0, HIGH);
    pinMode(A1, INPUT);
    digitalWrite(A1, HIGH);
    pinMode(A5, INPUT);
    digitalWrite(A5, HIGH);
}

void loop()
{
    if (!training)
    {
        Serial.println(">>>>Checking");
        // -1：重新被按下              之前松开了
        if (digitalRead(A0) == LOW)
        {
            --RM;
            delay(200);
        }
        // +1：重新被按下
        if (digitalRead(A1) == LOW)
        {
            ++RM;
            delay(200);
        }
        // Confirm Pressed
        if (digitalRead(A5) == LOW)
        {
            lastRM = RM;
            training = true;
            confirmBtnReleased = false;
            delay(200);
            Serial.println("Checking-Pressed");
        }
        if (RM > 20)
        {
            RM = 20;
        }
        if (RM < 1)
        {
            RM = 1;
        }
        char num[4];
        num[0] = '0' + group; // 第几组
        num[1] = '-';
        num[2] = '0' + RM / 10; // 个数
        num[3] = '0' + RM % 10;
        sevseg.setChars(num);
        sevseg.refreshDisplay(); // 必须重复运行刷新数码管显示
    }
    else
    { // 训练过程
        Serial.println("Training...");
        if (digitalRead(A5) == LOW)
        {
            training = false;
            delay(200);
            Serial.println("T-Pressed again.");
        }

        delaycount++;
        if (delaycount == 1000)
        { //通过变量自加来达到延时效果，如果使用delay函数则会打断数码管显示
            delaycount = 0;
            trainTime++;
        }

        if (trainTime > 9999)
        {
            trainTime = 0;
        }

        sevseg.setNumber(trainTime, -1); //设置要显示的数据，不显示小数点
        sevseg.refreshDisplay();         // 必须重复运行刷新数码管显示
    }
}