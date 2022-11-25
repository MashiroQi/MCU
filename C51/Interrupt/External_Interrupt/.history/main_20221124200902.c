#include <regx52.h>

void main()
{
    Exterinal_Interrupt_Init();
    while (1)
        ;
}

void Exterinal_Interrupt_Init()
{
}

void Exterinal_Interrupt_Function() 0
{
    P2_0 = !P2_0;
}