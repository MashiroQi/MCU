#include <regx52.h>
unsigned int count = 0;

void Timer_Interrput_Init();

void main()
{
    Timer_Interrput_Init();
    while (1);
}
//��ʱ������
void Timer_Interrput_Init()
{
    TMOD = 0x01;//���ö�ʱ��ģʽ
    TH0 = 0xfa; //���ö�ʱ����ʼֵ��λ
    TL0 = 0x00;//���ö�ʱ����ʼֵ��λ
    TR0 = 1;//���ö�ʱ��
    ET0 = 1;//������ʱ��
    EA = 1;//�������ж�
}
//��ʱ���жϴ����� �жϺ�1
void Timer_Intrrupt() interrupt 1
{

    TH0 = 0xfa;//���ö�ʱ����ʼֵ
    TL0 = 0x00;
    ++count;
    if (count % 1000 == 0)
    {
        P2_0 = !P2_0;
    }
}