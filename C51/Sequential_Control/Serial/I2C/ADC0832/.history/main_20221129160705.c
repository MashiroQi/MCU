#include <REG52.H>
#include <INTRINS.H>

sbit DI = P2 ^ 0;
sbit DO = P2 ^ 1;
sbit CLK = P2 ^ 2;
sbit CS = P2 ^ 3;

void Start()
{
    CLK = 0;
    CS = 0;
    DI = 1;
    CLK = 1;
    _nop_();
}

void main()
{
    while (1)
    {
    }
}
