#include <REG52.H>
#include "stdio.h"
sbit SCL = P2 ^ 1;
sbit SDA = P2 ^ 0;
void Serial_init();
unsigned char IIC_Read_Data(unsigned char addr);
void IIC_Write_Data(unsigned char addr, unsigned char dat);
void main()
{
    int i = 0;
    Serial_init();
    IIC_Write_Data(0x10, 0x0f);
    printf("%d\n", IIC_Read_Data(0x10 + i));
    while (1)
    {
    }
}
void delay(unsigned char x)
{
    while (x--)
        ;
}
void IIC_Start()
{
    SCL = 1;
    SDA = 1;
    delay(1);
    SCL = 0;
    delay(1);
    SDA = 0;
}
void IIC_Stop()
{
    SCL = 1;
    SDA = 0;
    delay(1);
    SDA = 0;
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
        SDA = dat & (0x80 >> count);
        delay(1);
        SCL = 0;
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
    IIC_WriteByte(0xa0); // ������ַ
    IIC_Wait_Ack();
    IIC_WriteByte(addr); //
    IIC_Wait_Ack();
    IIC_WriteByte(dat); //
    IIC_Wait_Ack();
    IIC_Stop();
    delay(1000);
}
unsigned char IIC_Read_Data(unsigned char addr)
{
    unsigned char dat = 0;
    IIC_Start();
    IIC_WriteByte(0xa0); // ������ַ
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
    TL1 = 0Xfd; // ���ò�����9600
    TR1 = 1;    // ����ʱ��1�ж�
    SM0 = 0;    // ������ʽ1
    SM1 = 1;
    ES = 1; // ��ʼ�����ж�
    EA = 1; // �����ж�
    TI = 1;
}