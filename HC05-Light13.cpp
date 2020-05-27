int LED = 13;

void setup()
{
    // put your setup code here, to run once:
    pinMode(LED, OUTPUT); // 定义13为输出引脚
    Serial.begin(9600);   //模块上电灯快闪，arduino发送指令时需要按住模块上的按键
                          //Serial.begin(38400);//按住按键再给模块上电，此时模块灯慢闪后即可松开按键
}

void sendcmd()
{
    Serial.println("AT"); //send cmd AT\r\n
    while (Serial.available())
    {
        char ch;
        ch = Serial.read();
        Serial.print(ch);
    }            // Get response: OK\r\n
    delay(1000); // wait for printing
}

void loop()
{
    //sendcmd();//循环发送AT指令，并接收反馈（测试模块AT指令时使用）
    Serial.println("start-----");
    while (Serial.available())
    {
        char ch;
        ch = Serial.read();
        Serial.println(ch);
        if (ch == '1')
        {
            digitalWrite(LED, HIGH); //接收到1亮灯
            break;
        }

        else if (ch == '0')
        {
            digitalWrite(LED, LOW); //接收到0灭灯
            break;
        }
        else
        {
            Serial.println("error cmd"); //错误指令不作任何操作并输出error
            break;
        }
    } // Get response
    Serial.println("end-----");
    delay(1000);
}