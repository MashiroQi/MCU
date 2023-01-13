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
    SCL = 0;
}