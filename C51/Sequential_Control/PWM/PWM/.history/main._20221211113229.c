#include <REG52.H>
#include "stdio.h"

sfr CMOD = 0xd9;
sfr CCON = 0xD8;
sfr CCAPM0 = 0xda;
sfr CCAPM1 = 0xdb;
sfr PCA_PWM0 = 0xf2;
sfr PCA_PWM1 = 0xf3;
sfr CCAP0L = 0xea;
sfr CCAP0H = 0xfa;
void PWM_Init()
{
    CMOD = 0x04;
    CCAPM0 = 0x42;
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

int main() // ������
{
    Serial_init(); // ���ڳ�ʼ��
    printf("PWM����\n");
    while (1)
    {
    }
}