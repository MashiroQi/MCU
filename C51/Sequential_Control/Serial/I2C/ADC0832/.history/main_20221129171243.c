
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
unsigned char Adc0832(unsigned char channel)
{
    uchar i = 0;
    uchar j;
    uint dat = 0;
    uchar ndat = 0;

    if (channel == 0)
        channel = 2;
    if (channel == 1)
        channel = 3;
    DI = 1;
    _nop_();
    _nop_();
    CS = 0;
    _nop_();
    _nop_();
    CLK = 1;
    _nop_();
    _nop_();
    CLK = 0;
    _nop_();
    _nop_();
    CLK = 1;
    DI = channel & 0x1;
    _nop_();
    _nop_();
    CLK = 0;
    _nop_();
    _nop_();
    CLK = 1;
    DI = (channel >> 1) & 0x1;
    _nop_();
    _nop_();
    CLK = 0;
    DI = 1;
    _nop_();
    _nop_();
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat |= DO;
        CLK = 1;
        _nop_();
        _nop_();
        CLK = 0;
        _nop_();
        _nop_();
        dat <<= 1;
        //        if(i==7)dat|=DO;
    }
    for (i = 0; i < 8; i++)
    {
        j = 0;
        j = j | DO;
        CLK = 1;
        _nop_();
        _nop_();
        CLK = 0;
        _nop_();
        _nop_();
        j = j << 7;
        ndat = ndat | j;
        if (i < 7)
            ndat >>= 1;
    }
    CS = 1;
    CLK = 0;
    DO = 1;
    dat <<= 8;
    dat |= ndat;
    return (dat);
}

void main()
{
    Serial_init();
    while (1)
    {
        printf("%ud\n", Adc0832(0));
        // printf("%f %f\n", Adc0832(0), Adc0832(1));
        delay(10000);
    }
}
