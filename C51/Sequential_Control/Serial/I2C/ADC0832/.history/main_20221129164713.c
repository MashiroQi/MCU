
#include <REG52.H>
#include <INTRINS.H>
#include <stdio.h>
typedef unsigned char uchar;
typedef unsigned int uint;

sbit DI = P2 ^ 0;
sbit DO = P2 ^ 1;
sbit CLK = P2 ^ 2;
sbit CS = P2 ^ 3;

void delay(uint x)
{
    while (x--)
        ;
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
unsigned char A_D(unsigned char CH)
{
    unsigned char i, test, adval;
    unsigned char dat = 0; // AD值
    adval = 0x00;
    test = 0x00;
    CLK = 0; // 初始化
    DI = 1;
    _nop_();
    CS = 0;
    _nop_();
    CLK = 1;
    _nop_();
    if (CH == 0x00) // 通道选择
    {
        CLK = 0;
        DI = 1; // 通道0的第一位
        _nop_();
        CLK = 1;
        _nop_();
        CLK = 0;
        DI = 0; // 通道0的第二位
        _nop_();
        CLK = 1;
        _nop_();
    }
    else
    {
        CLK = 0;
        DI = 1; // 通道1的第一位
        _nop_();
        CLK = 1;
        _nop_();
        CLK = 0;
        DI = 1; // 通道1的第二位
        _nop_();
        CLK = 1;
        _nop_();
    }

    CLK = 0;
    DI = 1;
    for (i = 0; i < 8; i++) // 读取前8位的值
    {
        _nop_();
        adval <<= 1;
        CLK = 1;
        _nop_();
        CLK = 0;
        if (DO)
            adval |= 0x01;
        else
            adval |= 0x00;
    }
    for (i = 0; i < 8; i++) // 读取后8位的值
    {
        test >>= 1;
        if (DO)
            test |= 0x80;
        else
            test |= 0x00;
        _nop_();
        CLK = 1;
        _nop_();
        CLK = 0;
    }
    nop_();
    CS = 1; // 释放ADC0832
    DO = 1;
    CLK = 1;
    return dat;
}
void main()
{
    Serial_init();
    while (1)
    {
        printf("%f", A_D(0));
        delay(111111);
    }
}
