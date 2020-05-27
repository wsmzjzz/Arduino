#include <Windows.h>
#include <iostream>
int main()
{
    HANDLE hCom = CreateFile("\\\\.\\COM7", GENERIC_READ | GENERIC_WRITE,
                             0, NULL, OPEN_EXISTING, 0, NULL); //与HC-06建立无线连接
    if (hCom == INVALID_HANDLE_VALUE)
    {
        std::cout << "Port unavailable!" << std::endl;
        return 0;
    }
    //以下配置串口通信参数
    DCB dcb;
    GetCommState(hCom, &dcb);
    dcb.BaudRate = 9600;
    dcb.ByteSize = DATABITS_8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    BOOL br = SetCommState(hCom, &dcb);
    COMMTIMEOUTS cto = {MAXDWORD, MAXDWORD, MAXDWORD, MAXDWORD, MAXDWORD};
    br = SetCommTimeouts(hCom, &cto);
    //开始等待用户输入
    for (;;)
    {
        int nInput = 0;
        std::cin >> nInput;
        if (nInput > 255 || nInput < 0)
        {
            break; //如果输入的值大于255或小于0则直接退出程序
        }
        BYTE byVal = (BYTE)nInput;
        DWORD dwTransmitted;
        //将输入的值发送给HC-06
        WriteFile(hCom, &byVal, sizeof(byVal), &dwTransmitted, NULL);
    }
    CloseHandle(hCom);
    return 0;
}