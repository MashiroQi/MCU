#include <regx52.h>
void Exterinal_Interrupt_Init();
void main()
{
    Exterinal_Interrupt_Init();
    while (1)
        ;
}

void Exterinal_Interrupt_Init()
{
    TI0 = 1; // 设置触发方式为下降沿触发
    EX0 = 1; // 开启外部中断0
    EA = 1;  // 开启总中断
}

void Exterinal_Interrupt_Function() interrupt 0
{
    P2_0 = !P2_0;
}