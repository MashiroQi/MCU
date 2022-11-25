#include <REGX51.H>
#include <stdio.h>
#include "intrins.h"

sbit SPI_Port = P3 ^ 7;
float Read_Temperture();
void delay(unsigned int x);
unsigned char Init();

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
    Init();
    while (1)
    {
        printf("%f\n", Read_Temperture());
        delay(1000);
    }
}
void delay(unsigned int x)
{
    while (x--)
        ;
}
void Reset()
{
    SPI_Port = 0;
    delay(75);
    SPI_Port = 1;
    delay(2);
}
unsigned char Check()
{
    unsigned char time = 0;
    while (SPI_Port && time < 20)
    {
        time++;
        delay(1);
    }
    if (time >= 20)
        return 1;
    else
        time = 0;
    while ((!SPI_Port) && time < 20)
    {
        time++;
        delay(1);
    }
    if (time >= 20)
        return 1;
    return 0;
}

unsigned char Read_Bit()
{
    unsigned char dat = 0;
    SPI_Port = 0;
    _nop_();
    _nop_();
    SPI_Port = 1;
    _nop_();
    _nop_();
    if (SPI_Port)
        dat = 1;
    else
        dat = 0;
    delay(5);
    return dat;
}

unsigned char Read_byte()
{
    unsigned char i = 0, dat = 0, temp = 0;
    for (i = 0; i < 8; i++)
    {
        temp = Read_Bit();
        dat = (temp << 7) | (dat >> 1);
    }
    return dat;
}
void Write_Byte(unsigned char dat)
{
    unsigned char i = 0, temp = 0;
    for (i = 0; i < 8; i++)
    {
        temp = dat & 0x01;
        dat >>= 1;
        if (temp)
        {
            SPI_Port = 0;
            _nop_();
            _nop_();
            SPI_Port = 1;
            delay(6);
        }
        else
        {
            SPI_Port = 0;
            delay(6);
            SPI_Port = 1;
            _nop_();
            _nop_();
        }
    }
}
void Start()
{
    Reset();
    Check();
    Write_Byte(0xcc);
    Write_Byte(0x44);
}
unsigned char Init()
{
    Reset();
    return Check();
}
float Read_Temperture()
{
    float temp;
    unsigned char datH = 0, datL = 0;
    unsigned int value = 0;
    Start();
    Reset();
    Check();
    Write_Byte(0xcc);
    Write_Byte(0xbe);
    datL = Read_byte();
    datH = Read_byte();
    value = (datH << 8) + datL;
    if ((value & 0xf800) == 0xf800)
    {
        value = (~value) + 1;
        temp = value * (-0.0625);
    }
    else
    {
        temp = value * 0.0625;
    }
    return temp;
}