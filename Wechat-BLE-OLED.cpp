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
boolean leftRlsed = true;
boolean rightRlsed = true;
boolean okRlsed = true;
int oledState = 222;

/**
 * 0: 
*/
// 全都用数组表示会导致OLED无法显示
// String part[5] = {"0.Chest", "1.Back", "2.Legs", "3.Arms"};
// String chest[] = {"0.Press", "1.Flies", "2.Pec-Deck", "3.Pushup"};
// String Arms[] = {"0.Bi-Curl", "1.Pushdown", "2.Press", "3.Raise"};
// String Legs[] = {"0.Squat", "1.Deadlift", "2.Press"};

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
boolean pressed(int btn)
{
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
    if (oledState == 111)
    {
        if (pressed(okBtn))
        {
            oledState = 222;
            workoutTime = 0;
            ++group;
            delay(200);
            goto NEXT;
        }
        if (pressed(leftBtn) && leftRlsed)
        {
            leftRlsed = false;
            --RM;
        }
        if (!pressed(leftBtn))
        {
            leftRlsed = true;
        }

        if (pressed(rightBtn) && rightRlsed)
        {
            rightRlsed = false;
            ++RM;
        }
        if (!pressed(rightBtn))
        {
            rightRlsed = true;
        }
        oled.clearDisplay(); //清屏

        output(2, 10, 30, "REP:");
        output(2, 70, 30, RM);

        oled.display(); // 开显示
    }
    if (oledState == 222)
    {
        if (pressed(okBtn))
        {
            oledState = 111;
            oled.clearDisplay(); //清屏
            output(2, 0, 25, "Break Time");
            oled.display();
            delay(1000);
            goto NEXT;
        }
        oled.clearDisplay(); //清屏

        output(2, 0, 0, "G");
        output(2, 0 + 13, 0, group);

        output(2, 45, 0, "R");
        output(2, 45 + 13, 0, RM);

        // String s = "hello";
        output(2, 0, 30, "time");
        ++workoutTime;
        output(2, 60, 30, workoutTime / 60);
        output(2, 60 + 13, 30, "'");
        output(2, 60 + 26, 30, workoutTime % 60);


        oled.display(); // 开显示

        delay(1000);
    }

NEXT:
    int a = 1;
}