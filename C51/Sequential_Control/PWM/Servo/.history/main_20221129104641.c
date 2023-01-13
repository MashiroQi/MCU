#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit PWM = P2 ^ 7;
uchar count, jd, i, j, ms;

void delay_50ms(uint x) // ��ʱ50ms    11.0592����
{
    TMOD = 0x01; // ����ʱ��0��������ʽΪ1
    TR0 = 1;     // ������ʱ��0��
    while (x--)
    {
        TH0 = (65536 - 46080) / 256; // ��ʱ50ms��ֵ�ĸ߰�λװ��TH0
        TL0 = (65536 - 46080) % 256; // ��ʱ50ms��ֵ�ĵͰ�λװ��TL0
        while (!TF0)
            ;    // �ȴ���ֱ��TF0Ϊ1
        TF0 = 0; // ���������־λ
    }
    TR0 = 0; // ֹͣ��ʱ��0��
}
void delay_500us(uint y) // ��ʱ500us
{
    TMOD = 0x10; // ����ʱ��1��������ʽΪ1
    TR1 = 1;     // ������ʱ��0��
    while (y--)
    {
        TH1 = (65536 - 461) / 256; // ��ʱ500us��ֵ�ĸ߰�λװ��TH0
        TL1 = (65536 - 461) % 256; // ��ʱ1ms��ֵ�ĵͰ�λװ��TL0
        while (!TF1)
            ;    // �ȴ���ֱ��TF1Ϊ1
        TF1 = 0; // ���������־λ
    }
    TR1 = 0; // ֹͣ��ʱ��1��
}

void main()
{

    while (1)
    {

        delay_50ms(100); // ��ʱ5s

        PWM = 1;
        delay_500us(1); // ��ʱ1ms����ת�Ƕ�-45��  ��ת
        PWM = 0;
        delay_500us(39);
        PWM = 1;
        delay_500us(1); // ��ʱ1ms����ת�Ƕ�-45��  ��ת
        PWM = 0;
        delay_500us(39);

        delay_50ms(100); // ��ʱ10s

        PWM = 0;
        delay_500us(2); // ��ʱ1ms����ת�Ƕ�-45��  ��ת
        PWM = 1;
        delay_500us(38);

        PWM = 0;
        delay_500us(2); // ��ʱ1.5ms����ת�Ƕ�0��  ��ת
        PWM = 1;
        delay_500us(38);

        delay_50ms(100); // ��ʱ10s
    }
}