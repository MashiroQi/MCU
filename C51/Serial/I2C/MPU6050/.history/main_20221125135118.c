#include <REG52.H>
void Serial_init();
void main()
{
    Serial_init();
    while (1)
    {
    }
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