#include <REGX51.H>
#include "intrins.h"
#include <>
sbit SPI_Port = P3 ^ 7;
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

int main() // ������
{
    Serial_init(); // ���ڳ�ʼ��
    while (1)
    {
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
        tmp = Read_Bit();
        dat = (temp << 7) | (dat >> 1);
    }
    return dat;
}
void Write_Byte(unsigned char dat)
{
    unsigned char i = 0, temp = 0;
    for (i = 0; i < 8; i++)
    {
        tmp = dat & 0x01;
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