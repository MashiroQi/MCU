#include <REGX51.H>
#include "stdio.h"

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
        if (!P2_0)
        {
            printf("P2 is %d\n", P2_0);
        }
        else
        {
            printf("P2 is High\n");
        }
    }
}