#include <REG52.H>
sbit SCL = P2 ^ 1;
sbit SDA = P2 ^ 0;

void main()
{
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

void IIC_WriteData(unsigned char dat)
{
    IIC_Start();
    IIC_SendByte(0x00); // Æ÷¼þµØÖ·
    IIC_Ask();
    IIC_SendByte(); //
    IIC_Ask();
    IIC_SendByte(); //
    IIC_Stop();
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
            SDA += 1;
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
