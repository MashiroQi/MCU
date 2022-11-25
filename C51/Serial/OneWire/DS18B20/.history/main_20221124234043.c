#include <REGX51.H>
#include "stdio.h"
sbit SPI = P3 ^ 7;
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
    while (1)
    {
    }
}
void delay(unsigned int x)
{
    while (x--)
        ;
}
