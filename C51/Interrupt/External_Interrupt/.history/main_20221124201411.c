#include <regx52.h>

void main()
{
    Exterinal_Interrupt_Init();
    while (1)
        ;
}

void Exterinal_Interrupt_Init()
{
    TI0 = 1; // ���ô�����ʽΪ�½��ش���
    EX0 = 1; // �����ⲿ�ж�0
    EA = 1;  // �������ж�
}

void Exterinal_Interrupt_Function() 0
{
    P2_0 = !P2_0;
}