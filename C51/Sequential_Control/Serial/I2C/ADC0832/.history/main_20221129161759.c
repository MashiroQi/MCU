
#include <REG52.H>
#include <INTRINS.H>

typedef unsigned char uchar;
typedef unsigned int uint;

sbit DI = P2 ^ 0;
sbit DO = P2 ^ 1;
sbit CLK = P2 ^ 2;
sbit CS = P2 ^ 3;

void Start(unsigned char CH)
{
    CLK = 0;
    CS = 0;
    DI = 1;
    CLK = 1;
    _nop_();
    _nop_();
    _nop_();
    CLK = 0;
    _nop_();
    _nop_();
    DI = 1;
    _nop_();
    CLK = 1;
    _nop_();
    _nop_();
    _nop_();
    CLK = 0;
    _nop_();
    _nop_();
    DI = 1;
    _nop_();
    CLK = 1;
    _nop_();
    _nop_();
    _nop_();
    CLK = 0;
    _nop_();
    _nop_();
    _nop_();
    CLK = 1;
    _nop_();
}
uchar ReadByte()
{
    unsigned char dat = 0, i = 0;
    for (i = 0; i < 8; i++)
    {
        CLK = 0;
        _nop_();
        _nop_();
        _nop_();
        CLK = 1;
        dat += DI;
        dat <<= 1;
    }
}

void main()
{
    while (1)
    {
    }
}
