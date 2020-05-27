/*
 * OLED显示
 */
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

int RM = 10;
int workoutTime = 0;

void setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(WHITE); //开像素点发光
    display.clearDisplay();      //清屏
}

void loop()
{
    display.clearDisplay(); //清屏

    display.setTextSize(1);   //设置字体大小
    display.setCursor(25, 5); //设置显示位置
    display.println("Group:   1");

    display.setTextSize(2);    //设置字体大小
    display.setCursor(15, 30); //设置显示位置
    display.println("Time: ");
    display.setTextSize(3);    //设置字体大小
    display.setCursor(80, 30); //设置显示位置
    String tm = "" + workoutTime;
    display.println(++workoutTime);

    display.display(); // 开显示

    delay(100);
}