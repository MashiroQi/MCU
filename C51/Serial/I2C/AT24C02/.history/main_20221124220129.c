#include <REG52.H>
sbit SCL = P2 ^ 1;
sbit SDA = P2 ^ 0;

void main()
{
}
void IIC_Start()
{
    SDA = 0;
    SCL = 1;
    SDA = 1;
}
void IIC_Stop()
{
    SDA = 1;
    SCL = 1;
    SDA = 0;
}
void IIC_SendByte(unsigned char dat)
{
    int count = 0;
    for (count = 0; count < 8; count++)
    {
        SDA = dat & (0x80 >> i);
        SCL = 0;
        SCL = 1;
        SCL = 0;
    }
}