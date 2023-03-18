#include <REGX51.H>
#include "stdio.h"

#define BUFFER_SIZE 64
char buffer[BUFFER_SIZE] = {0};
int buffer_arr;
char ReceiveFlag;
// 串口中断初始化
void Serial_init()
{
    TMOD |= 0X20;
    TH1 = 0Xfd;
    TL1 = 0Xfd; // 设置波特率9600
    TR1 = 1;    // 开定时器1中断
    SM0 = 0;    // 工作方式1
    SM1 = 1;
    ES = 1; // 开始串口中断
    EA = 1; // 开总中断
    TI = 1;
}

int main() // 主函数
{
    Serial_init(); // 串口初始化
    ReceiveFlag = 0;
    while (1)
    {
        if (ReceiveFlag)
        {
            printf("%s", buffer);
        }
    }
}
void SerialInterrpt() interrupt 4
{
    char rec;
    rec = SBUF;
    buffer[buffer_arr] = rec;
    buffer_arr++;
    RI = 0;

    if (buffer_arr == BUFFER_SIZE)
    {
        buffer_arr = 0; // 重新接收;
    }
    if (rec == '\n' || rec == '\0')
    {
        buffer[buffer_arr] = '\0';
        buffer_arr = 0; // 重新接收
        ReceiveFlag = 1;
    }
}
