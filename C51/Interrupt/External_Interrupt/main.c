#include <regx52.h>
//regx52.h���и��ඨ�����P2_0����ֱ��ʹ��
void Exterinal_Interrupt_Init();
void main()
{
    Exterinal_Interrupt_Init();
    while (1);
}
//�ⲿ�ж�����
void Exterinal_Interrupt_Init()
{
    IT0 = 1; // ���ô�����ʽΪ�½��ش���
    EX0 = 1; // �����ⲿ�ж�0
    EA = 1;  // �������ж�
}
//�ⲿ�жϴ����� �жϺ�Ϊ0
void Exterinal_Interrupt_Function() interrupt 0
{
    P2_0 = !P2_0;
}