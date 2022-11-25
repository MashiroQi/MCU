#include <REG52.H>

sbit SDA = ;
sbit SCL = ;

void Serial_init();
void main()
{
    Serial_init();
    while (1)
    {
    }
}
void delay(unsigned int x)
{
    while (x--)
        ;
}
void IIC_Start()
{
    SDA = 1;
    SCL = 1;
    delay(1);
    SDA = 0;
    delay(1);
    SCL = 0;
}
void IIC_Stop()
{
    SDA = 0;
    SCL = 1;
    delay(1);
    SDA = 1;
}

unsigned char Ack()
{
    unsigned char re;
    SDA = 1;
    delay(1);
    SCL = 1;
    delay(1);
    if (SDA)
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    SCL = 0;
    delay(1);
    return re;
}
void NoAck()
{
}
void Send_Byte(unsigned char dat)
{
    unsigned char i = 0;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
        {
            SDA = 1;
        }
        else
        {
            SDA = 0;
        }
        delay(1);
        SCL = 1;
        delay(1);
        SCL = 0;
        if (i == 7)
        {
            SDA = 1;
        }
        dat <<= 1;
        delay(1);
    }
}

unsigned char IIC_Read_Byte(unsigned char ack)
{
    unsigned char i = 0;
    unsigned char dat;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        SCL = 1;
        delay(1);
        if (SDA == 1)
        {
            dat += 1;
        }
    }
    if (ack)
    {
        ACK();
    }
    else
    {
        NoAck();
    }
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