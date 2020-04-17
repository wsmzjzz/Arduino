/*
 * OLED显示
 */
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET 4
Adafruit_SSD1306 oled(128, 64, &Wire, OLED_RESET);

int group = 4;
int lb = 5;
int RM = 10;
int workoutTime = 0;
int leftBtn = 12;
int rightBtn = 11;
int okBtn = 10;
bool leftRlsed = true;
bool rightRlsed = true;
bool okRlsed = true;

/**
 * 0: 
*/
int oledState = 0;
String part[5] = {"Chest", "Shoulders", "Back", "Legs", "Core"};

void output(int size, int x, int y, int num)
{
    oled.setTextSize(size); //设置字体大小
    oled.setCursor(x, y);   //设置显示位置
    oled.print(num);
    //    oled.display();
}
void output(int size, int x, int y, String str)
{
    oled.setTextSize(size); //设置字体大小
    oled.setCursor(x, y);   //设置显示位置
    oled.print(str);
    //    oled.display();
}
bool pressed(int btn) {
    return digitalRead(btn) == LOW;
}

void setup()
{
    Serial.begin(9600);

    pinMode(leftBtn, INPUT);
    digitalWrite(leftBtn, HIGH);
    pinMode(rightBtn, INPUT);
    digitalWrite(rightBtn, HIGH);
    pinMode(okBtn, INPUT);
    digitalWrite(okBtn, HIGH);

    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.setTextColor(WHITE); //开像素点发光
    oled.clearDisplay();      //清屏
    output(2, 10, 25, "Optimism");
    oled.display();
    // delay(2000);
}

void loop()
{
    if (pressed(leftBtn) && leftRlsed)
    {
        leftRlsed = false;
        workoutTime -= 10;
        // delay(150);
        // oled.display(); // 开显示
        // goto NEXT;
    }
    if (!pressed(leftBtn)) {
        leftRlsed = true;
    }

    if (pressed(rightBtn) && rightRlsed)
    {
        rightRlsed = false;
        workoutTime += 10;
        // delay(150);
        // output(2, 60, 30, ++workoutTime);
        // oled.display(); // 开显示
        // goto NEXT;
    }
    if (!pressed(rightBtn)) {
        rightRlsed = true;
    }
    oled.clearDisplay(); //清屏

    output(2, 0, 0, "G");
    output(2, 0 + 13, 0, group);

    output(2, 30, 0, "R");
    output(2, 30 + 13, 0, RM);
    output(2, 60, 30, workoutTime);

    oled.display(); // 开显示

    // delay(1000);

NEXT:;
}