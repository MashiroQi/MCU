#include <REG52.H>

void delay(unsigned int x)
{
    int i, j;
    for (i = 0; i < x; i++)
        j = 110;
    while (j--)
        ;
}
void main()
{
}
