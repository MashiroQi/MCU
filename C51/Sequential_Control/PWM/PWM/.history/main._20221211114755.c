#include <REG52.H>
#include "stdio.h"

#define FOSC 18432000L
typedef unsigned char BYTE;
typedef unsigned int WORD;
sfr CCON = 0xD8;
sbit CCF0 = CCON ^ 0;
sbit CCF1 = CCON ^ 1;
sbit CR = CCON ^ 6;
sbit CF = CCON ^ 7;
sfr CMOD = 0xD9;
sfr CL = 0xE9;
sfr CH = 0xF9;
sfr CCAPM0 = 0xDA;
sfr CCAP0L = 0xEA;
sfr CCAP0H = 0xFA;
sfr CCAPM1 = 0xDB;
sfr CCAP1L = 0xEB;
sfr CCAP1H = 0xFB;
sfr PCAPWM0 = 0xf2;
sfr PCAPWM1 = 0xf3;
void PWM_Init()
{
    CCON = 0;
    CL = 0;
    CH = 0;
    CMOD = 0x02;
    CCAP0H = CCAP0L = 0x80;
    CCAPM0 = 0x42;
    CCAP1H = CCAP1L = 0xff;
    PCAPWM1 = 0x03;
    CCAPM1 = 0x42;
    CR = 1;
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

void main()
{
    Serial_init();
    PWM_Init();
    while (1)
        ;
}