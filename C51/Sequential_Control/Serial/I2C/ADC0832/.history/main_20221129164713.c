
#include <REG52.H>
#include <INTRINS.H>
#include <stdio.h>
typedef unsigned char uchar;
typedef unsigned int uint;

sbit DI = P2 ^ 0;
sbit DO = P2 ^ 1;
sbit CLK = P2 ^ 2;
sbit CS = P2 ^ 3;

void delay(uint x)
{
    while (x--)
        ;
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
unsigned char A_D(unsigned char CH)
{
    unsigned char i, test, adval;
    unsigned char dat = 0; // ADֵ
    adval = 0x00;
    test = 0x00;
    CLK = 0; // ��ʼ��
    DI = 1;
    _nop_();
    CS = 0;
    _nop_();
    CLK = 1;
    _nop_();
    if (CH == 0x00) // ͨ��ѡ��
    {
        CLK = 0;
        DI = 1; // ͨ��0�ĵ�һλ
        _nop_();
        CLK = 1;
        _nop_();
        CLK = 0;
        DI = 0; // ͨ��0�ĵڶ�λ
        _nop_();
        CLK = 1;
        _nop_();
    }
    else
    {
        CLK = 0;
        DI = 1; // ͨ��1�ĵ�һλ
        _nop_();
        CLK = 1;
        _nop_();
        CLK = 0;
        DI = 1; // ͨ��1�ĵڶ�λ
        _nop_();
        CLK = 1;
        _nop_();
    }

    CLK = 0;
    DI = 1;
    for (i = 0; i < 8; i++) // ��ȡǰ8λ��ֵ
    {
        _nop_();
        adval <<= 1;
        CLK = 1;
        _nop_();
        CLK = 0;
        if (DO)
            adval |= 0x01;
        else
            adval |= 0x00;
    }
    for (i = 0; i < 8; i++) // ��ȡ��8λ��ֵ
    {
        test >>= 1;
        if (DO)
            test |= 0x80;
        else
            test |= 0x00;
        _nop_();
        CLK = 1;
        _nop_();
        CLK = 0;
    }
    nop_();
    CS = 1; // �ͷ�ADC0832
    DO = 1;
    CLK = 1;
    return dat;
}
void main()
{
    Serial_init();
    while (1)
    {
        printf("%f", A_D(0));
        delay(111111);
    }
}
