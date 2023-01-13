#include <REG52.H>
sbit GND = P0 ^ 0;
sbit VCC = P0 ^ 2;
sbit PWM = P0 ^ 1;
void delay(unsigned int x)
{
    int i, j;
    for (i = 0; i < x; i++)
    {
        j = 50;
        while (j--)
            ;
    }
}
void main()
{
}
