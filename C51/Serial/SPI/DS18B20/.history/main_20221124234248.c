#include <REGX51.H>
#include "stdio.h"
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
void ds18b20_reset()
{
    SPI_Port = 0;
    delay(75);
    SPI_Port = 1;
    delay(2);
}
void ds18b20_check()
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
}
