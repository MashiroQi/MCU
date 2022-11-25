#include <regx52.h>
unsigned int count = 0;
void main()
{
    Timer_Interrput_Init();
    while (1)
        ;
}

void Timer_Interrput_Init()
{
    TMOD = 0x01;
    TH0 = 0xfa;
    TL0 = 0x00;
    TR0 = 1;
    ET0 = 1;
    EA = 1;
}
void Timer_Intrrupt() interrupt 1
{
    TH0 = 0xfa;
    TL0 = 0x00;
    if (count % 1000 == 0)
    {
        P2_0 = !P2_0;
    }
}