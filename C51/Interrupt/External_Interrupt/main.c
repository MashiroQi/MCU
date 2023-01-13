#include <regx52.h>
//regx52.h中有更多定义好了P2_0可以直接使用
void Exterinal_Interrupt_Init();
void main()
{
    Exterinal_Interrupt_Init();
    while (1);
}
//外部中断设置
void Exterinal_Interrupt_Init()
{
    IT0 = 1; // 设置触发方式为下降沿触发
    EX0 = 1; // 开启外部中断0
    EA = 1;  // 开启总中断
}
//外部中断处理函数 中断号为0
void Exterinal_Interrupt_Function() interrupt 0
{
    P2_0 = !P2_0;
}