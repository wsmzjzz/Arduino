/************************************************
 我的硬件-微信小程序 Arduino 蓝牙通信示范例程
 技术支持：635404055（QQ群）
 关注微信公众号："hi iot"，免费获取资料。
 我们将在公众号内定期推送各种有趣好玩的物联网、创意作品。
 作者：@DTJ
************************************************/

int red_ledPin = 13;   //定义数字10 接口
int yellow_ledPin = 6; //定义数字10 接口
int green_ledPin = 7;  //定义数字10 接口
int blue_ledPin = 8;   //定义数字10 接口
int white_ledPin = 9;  //定义数字10 接口
int potpin = 5;        //定义模拟接口5

#define red "010200"
#define blue "020201"
#define green "020202"
#define yellow "020203"
#define white "020204"
#define slider0 "020300"
#define vol "020507"

//格式化发送
void wxxcx_send(String addr, String data)
{
    String sendbuff = "{#" + addr + ":" + data + "}"; //格式:{#控件ID:内容}
    Serial.print(sendbuff);
}
//非协议数据处理
void wxxcx_general_deal(String data)
{
    int val = 0;

    if (data == "get") //按键发送过来的自定义数据
    {
        val = analogRead(potpin); //读取模拟接口5 的值，并将其赋给val
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
            digitalWrite(red_ledPin, HIGH); //点亮小灯
        }
        else
        {
            digitalWrite(red_ledPin, LOW); //熄灭小灯
        }
    }
    else if (addr == blue)
    {
        if (ctent == "true")
        {
            digitalWrite(blue_ledPin, HIGH); //点亮小灯
        }
        else
        {
            digitalWrite(blue_ledPin, LOW); //熄灭小灯
        }
    }
    else if (addr == green)
    {
        if (ctent == "true")
        {
            digitalWrite(green_ledPin, HIGH); //点亮小灯
        }
        else
        {
            digitalWrite(green_ledPin, LOW); //熄灭小灯
        }
    }
    else if (addr == yellow)
    {
        if (ctent == "true")
        {
            digitalWrite(yellow_ledPin, HIGH); //点亮小灯
        }
        else
        {
            digitalWrite(yellow_ledPin, LOW); //熄灭小灯
        }
    }
    else if (addr == white)
    {
        if (ctent == "true")
        {
            digitalWrite(white_ledPin, HIGH); //点亮小灯
        }
        else
        {
            digitalWrite(white_ledPin, LOW); //熄灭小灯
        }
    }
    else if (addr == slider0)
    {
        //滑条的数据是十进制数，将 ctent 转为十进制即可
    }
}
//“我的硬件”数据解析函数
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

void setup()
{
    Serial.begin(9600);          //设置波特率为9600
    pinMode(red_ledPin, OUTPUT); //定义小灯接口为输出接口
    //  pinMode(yellow_ledPin, OUTPUT);//定义小灯接口为输出接口
    //  pinMode(green_ledPin, OUTPUT);//定义小灯接口为输出接口
    //  pinMode(blue_ledPin, OUTPUT);//定义小灯接口为输出接口
    //  pinMode(white_ledPin, OUTPUT);//定义小灯接口为输出接口
}
void loop()
{
    String comdata = "";

    while (Serial.available() > 0)
    {
        comdata += char(Serial.read());
        delay(2);
    }
    if (comdata.length() > 0)
    {
        Serial.println(comdata);
        wxxcx_analysis(comdata);
        comdata = "";
    }
}
