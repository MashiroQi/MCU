#include <REG52.H>
sbit GND = P0 ^ 0;
sbit VCC = P0 ^ 2;
sbit PWM = P0 ^ 1;
void delay(unsigned int x)
{
    int i, j;
    for (i = 0; i < x; i++)
    {
        j = 10;
        while (j--)
            ;
    }
}
void main()
{
    GND = 0;
    VCC = 1;
    while (1)
    {
        int i = 0;
        for (i = 0; i < 200; i++)
        {
            delay(i);
            PWM = 1;
            delay(200 - i);
            PWM = 0;
        }
    }
}
