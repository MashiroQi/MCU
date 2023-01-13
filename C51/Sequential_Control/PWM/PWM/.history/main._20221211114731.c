#include <REG52.H>
#include "stdio.h"

#define FOSC 18432000L
typedef unsigned char BYTE;
typedef unsigned int WORD;
sfr CCON = 0xD8;      // PCA control register
sbit CCF0 = CCON ^ 0; // PCA module-0 interrupt flag
sbit CCF1 = CCON ^ 1; // PCA module-1 interrupt flag
sbit CR = CCON ^ 6;   // PCA timer run control bit
sbit CF = CCON ^ 7;   // PCA timer overflow flag
sfr CMOD = 0xD9;      // PCA mode register
sfr CL = 0xE9;        // PCA base timer LOW
sfr CH = 0xF9;        // PCA base timer HIGH
sfr CCAPM0 = 0xDA;    // PCA module-0 mode register
sfr CCAP0L = 0xEA;    // PCA module-0 capture register LOW
sfr CCAP0H = 0xFA;    // PCA module-0 capture register HIGH
sfr CCAPM1 = 0xDB;    // PCA module-1 mode register
sfr CCAP1L = 0xEB;    // PCA module-1 capture register LOW
sfr CCAP1H = 0xFB;    // PCA module-1 capture register HIGH
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