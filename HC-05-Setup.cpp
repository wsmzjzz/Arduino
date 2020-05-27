//在这里我们导入SoftwareSerial.h
#include <SoftwareSerial.h>

//初始一个软件串口 serial2(Txd, Rxd)
SoftwareSerial serial2(6, 7);

void setup()
{
    //连接电脑串口
    Serial.begin(9600);
    //连接hc-05串口,AT模式默认串口速度是38400:
    serial2.begin(38400);
    Serial.println("init serial port AT");
}

void loop()
{
    //把电脑输入串口的字符 写入到 hc-05 的串口中
    if (Serial.available())
        serial2.write(Serial.read());
    //把hc-05的串口输出的字符 输出到电脑串口中
    if (serial2.available())
        Serial.write(serial2.read());
}