#include <REG52.H>
#include "stdio.h"
sbit SCL = P2 ^ 1;
sbit SDA = P2 ^ 0;
void delay_ms(unsigned int ms);
void Serial_init();
unsigned char IIC_Read_Data(unsigned char addr);
void IIC_Write_Data(unsigned char addr, unsigned char dat);
void main()
{
    Serial_init();
    IIC_Write_Data(0, 0x0f);
    delay_ms(10);
    printf("%d\n", IIC_Read_Data(0));

    while (1)
    {
    }
}
void delay(unsigned int x)
{
    while (x--)
        ;
}
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}
void IIC_Start()
{
    SCL = 1;
    SDA = 1;
    delay(1);
    SDA = 0;
    delay(1);
    SCL = 0;
}
void IIC_Stop()
{
    SCL = 1;
    SDA = 0;
    delay(1);
    SDA = 1;
    delay(1);
}

void IIC_Ask()
{
    SCL = 0;
    SDA = 0;
    delay(1);
    SCL = 1;
    delay(1);
    SCL = 0;
}
void IIC_NoAsk()
{
    SCL = 0;
    SDA = 1;
    delay(1);
    SCL = 1;
    delay(1);
    SCL = 0;
}

unsigned char IIC_Wait_Ack()
{
    unsigned char time = 0;
    SCL = 1;
    delay(1);
    while (SDA)
    {
        time++;
        if (time > 100)
        {
            IIC_Stop();
            return 1;
        }
    }
    SCL = 0;
    return 0;
}

void IIC_WriteByte(unsigned char dat)
{
    int count = 0;
    SCL = 0;
    for (count = 0; count < 8; count++)
    {
        if ((dat & 0x80) > 0)
        {
            SDA = 1;
        }
        else
        {
            SDA = 0;
        }
        dat <<= 1;
        delay(1);
        SCL = 1;
        delay(1);
        SCL = 0;
        delay(1);
    }
}
unsigned char IIC_ReadByte(unsigned char ack)
{
    unsigned char dat = 0;
    unsigned char count = 0;
    for (count = 0; count < 8; count++)
    {
        SCL = 0;
        delay(1);
        SCL = 1;
        dat <<= 1;
        if (SDA)
        {
            dat += 1;
        }
        delay(1);
        if (ack)
        {
            IIC_Ask();
        }
        else
        {
            IIC_NoAsk();
        }
    }
    return dat;
}

void IIC_Write_Data(unsigned char addr, unsigned char dat)
{
    IIC_Start();
    IIC_WriteByte(0xa0); // 器件地址
    IIC_Wait_Ack();
    IIC_WriteByte(addr); //
    IIC_Wait_Ack();
    IIC_WriteByte(dat); //
    IIC_Wait_Ack();
    IIC_Stop();
    delay_ms(10);
}
unsigned char IIC_Read_Data(unsigned char addr)
{
    unsigned char dat = 0;
    IIC_Start();
    IIC_WriteByte(0xa0); // 器件地址
    IIC_Wait_Ack();
    IIC_WriteByte(addr); //
    IIC_Wait_Ack();
    IIC_Start();
    IIC_WriteByte(0xa1); //
    IIC_Wait_Ack();
    dat = IIC_ReadByte(0);
    IIC_Stop();
    return dat;
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