#include <regx52.h>
void main()
{
}

void Timer_Interrput_Init()
{
    TMOD = 0x01;
    TL0 = 0x00;
    TH0 = 0xfa;
    TR0 = 1;
    ET0 = 1;
    EA = 1;
}
void Timer_Intrrupt() interrupt 1
{
}