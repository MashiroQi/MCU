
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

void Start(unsigned char CH)
{
    CLK = 0;
    CS = 0;
    DI = 1;
    CLK = 1;
    _nop_();
    _nop_();
    _nop_();
    CLK = 0;
    _nop_();
    _nop_();
    DI = 1;
    _nop_();
    CLK = 1;
    _nop_();
    _nop_();
    _nop_();
    CLK = 0;
    _nop_();
    _nop_();
    if (CH = 0)
        DI = 0;
    if (CH = 1)
        DI = 1;
    _nop_();
    CLK = 1;
    _nop_();
    _nop_();
    _nop_();
    CLK = 0;
    _nop_();
    _nop_();
    _nop_();
    CLK = 1;
    _nop_();
}
uchar ReadByte()
{
    unsigned char dat = 0, i = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        CLK = 0;
        _nop_();
        _nop_();
        _nop_();
        CLK = 1;
        dat += DI;
        CLK = 0;
        _nop_();
        _nop_();
        _nop_();
    }
    return dat;
}

uchar ReadData(uchar CH)
{
    Start(CH);
    return ReadByte();
}
void main()
{
    Serial_init();
    while (1)
    {
    }
}
