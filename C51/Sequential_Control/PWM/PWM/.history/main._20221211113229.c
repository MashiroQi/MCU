#include <REG52.H>
#include "stdio.h"

sfr CMOD = 0xd9;
sfr CCON = 0xD8;
sfr CCAPM0 = 0xda;
sfr CCAPM1 = 0xdb;
sfr PCA_PWM0 = 0xf2;
sfr PCA_PWM1 = 0xf3;
sfr CCAP0L = 0xea;
sfr CCAP0H = 0xfa;
void PWM_Init()
{
    CMOD = 0x04;
    CCAPM0 = 0x42;
}

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
    printf("PWM启动\n");
    while (1)
    {
    }
}