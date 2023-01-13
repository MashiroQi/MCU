#include <REGX51.H>
#include "stdio.h"

sfr ADC_CONTR = 0xbc;
sfr ADC_RES = 0xbe;
sfr ADC_RESL = 0x97;

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
unsigned int ADC(unsigned char channel)
{
    ADC_CONTR = 0xe8 | (channel & 0x7);
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    while (ADC_CONTR & 0x10)
        ;
    return (unsigned int)((unsigned int)(ADC_RES << 2) | (unsigned int)(ADC_RESL));
}

int main() // ������
{
    Serial_init(); // ���ڳ�ʼ��
    while (1)
    {
    }
}