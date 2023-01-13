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
void delay_100us(uint y) // ��ʱ500us
{
    TMOD = 0x10; // ����ʱ��1��������ʽΪ1
    TR1 = 1;     // ������ʱ��0��
    while (y--)
    {
        TL0 = 0xAE; // ���ö�ʱ��ֵ
        TH0 = 0xFB; // ���ö�ʱ��ֵ
        while (!TF1)
            ;    // �ȴ���ֱ��TF1Ϊ1
        TF1 = 0; // ���������־λ
    }
    TR1 = 0; // ֹͣ��ʱ��1��
}

void Servo_PWM(unsigned char x)
{
    PWM = 1;
    delay_500us(x);
    PWM = 0;
    delay_500us(40 - x);
    PWM = 1;
    delay_500us(x);
    PWM = 0;
    delay_500us(40 - x);
}
void main()
{

    while (1)
    {

        Servo_PWM(1);
        delay_50ms(10);
        Servo_PWM(2);
        delay_50ms(10);
        Servo_PWM(3);
        delay_50ms(10);

        Servo_PWM(4);
        delay_50ms(10);
    }
}
