/*
 * OLED显示
 */
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET 4
Adafruit_SSD1306 oled(128, 64, &Wire, OLED_RESET);

int sets = 0; // 训练组数
int lb = 5;   // weight
int RM = 10;  // Repetition
int totalTime = 0;
int workoutTime = 0;      // each training time
int leftBtn = 12;         // minus- :
int rightBtn = 11;        // plus+ :
int okBtn = 10;           //
boolean leftRlsed = true; // ?Released
boolean rightRlsed = true;
boolean okRlsed = true;
boolean lbChoosed = false;
int oledState = 000; // 000:Menu 111:BreakTime 222:Training 444:End

String comdata = "";
// 绿光LED for testing
int ledPin = 13;
int trainItems[15] = {0, 1, 2, 3, 4};
int numTrainItems = 2;

// 小组件编号 使用宏定义节省全局变量存储空间
#define Press "000100"
#define IncPress "000101"
#define Flies "000102"
#define Pushup "000103"
#define Squat "000104"
#define Row "000105"
#define Pullup "000106"
#define Deadlift "000107"
#define Lunge "000108"
#define Situp "000109"
#define BiCurl "000110"
#define Pushdown "000111"
#define Delete "000112"
#define red "000200"
#define slider0 "000300"
#define vol "020507"
#define input0 "010400"

String itemName(int index)
{
    if (index == 0)
        return "Press";
    if (index == 1)
        return "IncPress";
    if (index == 2)
        return "Flies";
    if (index == 3)
        return "Pushup";
    if (index == 4)
        return "Squat";
    if (index == 5)
        return "Row";
    if (index == 6)
        return "Pullup";
    if (index == 7)
        return "Deadlift";
    if (index == 8)
        return "Lunge";
    if (index == 9)
        return "Situp";
    if (index == 10)
        return "BiCurl";
    if (index == 11)
        return "PsDown";
}
//格式化发送
void wxxcx_send(String addr, String data)
{
    String sendbuff = "{#" + addr + ":" + data + "}"; //格式:{#控件ID:内容}
    Serial.println(sendbuff);
}
//非协议数据处理
void wxxcx_general_deal(String data)
{
    int val = 0;

    if (data == "delete")
    {
        --numTrainItems;
    } else {
        ++numTrainItems;
        if (data == "pr") trainItems[numTrainItems] = 0;
        if (data == "fl") trainItems[numTrainItems] = 2;
        if (data == "psup") trainItems[numTrainItems] = 3;
        // if (data == "sq") trainItems[numTrainItems] = 4;
        // if (data == "rw") trainItems[numTrainItems] = 5;
        // if (data == "plup") trainItems[numTrainItems] = 6;
        // if (data == "ddlf") trainItems[numTrainItems] = 7;
        // if (data == "lg") trainItems[numTrainItems] = 8;
        // if (data == "stup") trainItems[numTrainItems] = 9;
        // if (data == "bicl") trainItems[numTrainItems] = 10;
        // if (data == "psdn") trainItems[numTrainItems] = 11;
    }

    // else if (data == "incPress")
    // {
    //     trainContent[++ptrTrainCtnt] = 2;
    // }
    if (data == "get") //按键发送过来的自定义数据
    {
        //val = analogRead(potpin); //读取模拟接口5 的值，并将其赋给val
        wxxcx_send(vol, String(val));
    }
}
//协议数据处理
void wxxcx_protocol_deal(String addr, String ctent)
{
    if (addr == red)
    {
        if (ctent == "true")
        {
            delay(100);
            wxxcx_send(input0, "Hello");
            digitalWrite(ledPin, HIGH); //点亮小灯
        }
        else
        {
            digitalWrite(ledPin, LOW); //熄灭小灯
        }
    }

    else if (addr == slider0)
    {
        //滑条的数据是十进制数，将 ctent 转为十进制即可
        int data = ctent.toInt();

        if (data > 10)
        {
            digitalWrite(ledPin, HIGH); //点亮小灯
            wxxcx_send("010700", String(data));
            Serial.println("***Sent");
        }
        else
        {
            digitalWrite(ledPin, LOW); //熄灭小灯
        }
    }
}
//“我的硬件”数据解析函数---不要动这个
void wxxcx_analysis(String str)
{
    String address = "";
    String content = "";
    int first_index = 0;
    int last_index = 0;
    int end_index = 0;

    //协议数据
    while ((first_index = str.indexOf('#', first_index + 1)) > 0)
    {
        last_index = str.indexOf(':', last_index + 1);
        end_index = str.indexOf('}', end_index + 1);

        if (end_index < last_index)
        {
            continue;
        }

        if ((last_index - first_index) == 7) //地址长度是否正确
        {
            address = str.substring(first_index + 1, last_index); //获取控件ID
            content = str.substring(last_index + 1, end_index);   //获取控件内容
            wxxcx_protocol_deal(address, content);
            //Serial.println(content);
            //对比控件ID
        }
    }
    if (end_index == 0)
    {
        //如果是按键的自定义数据则会到这,自行解析
        wxxcx_general_deal(str);
    }
}
// 打印到OLED屏幕，配合display()显示
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
// 按钮按下-电位变低
boolean pressed(int btn)
{
    return digitalRead(btn) == LOW;
}
// -&+button: change number
void onButtonEvent(int &num, int lowBound, int upBound)
{
    if (pressed(leftBtn) && leftRlsed) // pressed again
    {
        leftRlsed = false;
        --num;
        if (num < lowBound)
            num = lowBound; // min:1
    }
    if (!pressed(leftBtn))
    {
        leftRlsed = true;
    }

    if (pressed(rightBtn) && rightRlsed) // pressed again
    {
        rightRlsed = false;
        ++num;
        if (num > upBound)
            num = upBound; // max:25
    }
    if (!pressed(rightBtn))
    {
        rightRlsed = true;
    }
}
//-----------------------------------------------
void setup()
{
    Serial.begin(9600);
    // 13 pin
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    // init of buttons' pins
    pinMode(leftBtn, INPUT);
    digitalWrite(leftBtn, HIGH);
    pinMode(rightBtn, INPUT);
    digitalWrite(rightBtn, HIGH);
    pinMode(okBtn, INPUT);
    digitalWrite(okBtn, HIGH);
    // OLED init
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.setTextColor(WHITE); //开像素点发光
    oled.clearDisplay();      //清屏
    output(2, 10, 25, "Optimism");
    oled.display();
    delay(500);
}
//----------------------------------------------
void loop()
{
    // Bluetooth info exchange

    // read info from HC-08(from Wechat) in one while loop
    // while (Serial.available() > 0)
    // {
    //     comdata += char(Serial.read());
    //     delay(2);
    // }
    // if (comdata.length() > 0)
    // {
    //     Serial.println(comdata);
    //     // do whatever
    //     wxxcx_analysis(comdata);
    //     comdata = "";
    // }
    // Menu
    if (oledState == 000)
    {
        if (pressed(okBtn))
        {
            okRlsed = false;
            oledState = 111;
            // -----init-----
            sets = 0;
            lb = 10;
            RM = 10;
            lbChoosed = false;
            totalTime = 0;
            // delay(200);
        }
        oled.clearDisplay(); //清屏
        // output(2, 0, 0, "Menu");
        for (int i = 1; i <= numTrainItems; ++i)
        {
            output(2, 0, 15 * (i-1), itemName(trainItems[i]));
        }
        oled.display();
        // delay(1500);

    } // end of Menu
    // BreakTime
    if (oledState == 111)
    {
        // Combination shortcut to 'Fin.': -&ok | +&ok
        if (pressed(leftBtn) && leftRlsed && pressed(okBtn) ||
            pressed(rightBtn) && rightRlsed && pressed(okBtn))
        {
            oledState = 444;

            oled.clearDisplay(); //清屏
            output(3, 20, 20, "DONE!");
            oled.display();
            delay(1000);
        }
        // to 'Training'
        if (!pressed(okBtn))
            okRlsed = true;
        if (pressed(okBtn) && okRlsed)
        {
            // first-time-press of 'ok': confirm LBS
            if (!lbChoosed && okRlsed)
            {
                lbChoosed = true;
                okRlsed = false;
            }
            // second-time: confirm REP & to Training
            if (lbChoosed && okRlsed)
            {
                oledState = 222; // 训练模式
                workoutTime = 0; // 单次/一组训练时间
                ++sets;          // 组数

                // Count down 3 2 1 GO
                oled.clearDisplay(); //清屏
                output(4, 60, 20, "3");
                oled.display();
                delay(1000);
                oled.clearDisplay(); //清屏
                output(4, 60, 20, "2");
                oled.display();
                delay(1000);
                oled.clearDisplay(); //清屏
                output(4, 60, 20, "1");
                oled.display();
                delay(1000);
                oled.clearDisplay(); //清屏
                output(4, 50, 20, "GO");
                oled.display();
                delay(1000);

                // delay(200);
                goto NEXT;
            }
        }
        // set LBS & REP
        if (!lbChoosed)
            onButtonEvent(lb, 5, 60);
        else
            onButtonEvent(RM, 1, 25);

        oled.clearDisplay(); //清屏
        if (!lbChoosed)
            output(2, 10, 15, ">");
        else
            output(2, 10, 35, ">");
        output(2, 25, 15, "LBS:");
        output(2, 75, 15, lb);
        output(2, 25, 35, "REP:");
        output(2, 75, 35, RM);
        oled.display();   // 开显示
    }                     // end of BreakTime
    if (oledState == 222) // Training Mode
    {
        if (pressed(okBtn)) // to 'Break'
        {
            oledState = 111;
            // Statistic
            totalTime += workoutTime;
            workoutTime = 0;
            lbChoosed = false;

            oled.clearDisplay(); //清屏
            output(4, 0, 18, "Break");
            oled.display();
            delay(1000);
            goto NEXT;
        }
        oled.clearDisplay(); //清屏
        // Gx Rxx
        output(2, 0, 0, "S");
        output(2, 0 + 13, 0, sets);
        output(2, 45, 0, "R");
        output(2, 45 + 13, 0, RM);
        // time x'xx
        output(2, 0, 30, "time");
        ++workoutTime;
        output(2, 60, 30, workoutTime / 60);
        output(2, 60 + 13, 30, "'");
        output(2, 60 + 26, 30, workoutTime % 60);

        oled.display(); // 开显示

        delay(1000); // time counting
    }                // end of Training
    // Finished
    if (oledState == 444)
    {
        // Press any key to 'Menu'
        if (pressed(leftBtn) || pressed(rightBtn) || pressed(okBtn))
        {
            oledState = 000;
            // End words
            oled.clearDisplay(); //清屏
            output(2, 0, 0, "Keep it up & Go for it");
            oled.display();
            delay(1500);
            // delay(200);
        }
        // End statistics
        oled.clearDisplay(); //清屏
        output(2, 0, 0, "Time:");
        output(2, 30, 30, totalTime / 60);
        output(2, 30 + 13, 30, "'");
        output(2, 30 + 26, 30, totalTime % 60);
        oled.display();
        // delay(1000);

    } // end of finished

NEXT:
    int a = 1;
}