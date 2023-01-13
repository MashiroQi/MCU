#include <regx52.h>
unsigned int count = 0;

void Timer_Interrput_Init();

void main()
{
    Timer_Interrput_Init();
    while (1);
}
//定时器设置
void Timer_Interrput_Init()
{
    TMOD = 0x01;//设置定时器模式
    TH0 = 0xfa; //设置定时器初始值高位
    TL0 = 0x00;//设置定时器初始值低位
    TR0 = 1;//启用定时器
    ET0 = 1;//开启定时器
    EA = 1;//开启总中断
}
//定时器中断处理函数 中断号1
void Timer_Intrrupt() interrupt 1
{

    TH0 = 0xfa;//设置定时器初始值
    TL0 = 0x00;
    ++count;
    if (count % 1000 == 0)
    {
        P2_0 = !P2_0;
    }
}