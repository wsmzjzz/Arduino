#include <SoftwareSerial.h>
// 设置软串口使用的针脚
SoftwareSerial softSerial(7, 8); //7为RX, 8为TX
int led = 3;
void setup()
{
    Serial.begin(9600);     //设定硬串口波特率
    softSerial.begin(9600); //设定软串口波特率
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
}
void loop()
{
    if (softSerial.available())
    {   //如果HC-06发来数据
           int k = softSerial.read(); //读取1个字节的数据
        //    Serial.println(k); //通过硬串口打印输出
        if (k == 1)
            digitalWrite(led, HIGH);
        else
            digitalWrite(led, LOW);
    }
}