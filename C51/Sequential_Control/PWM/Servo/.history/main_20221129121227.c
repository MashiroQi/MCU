#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit PWM = P2 ^ 7;
uchar count, jd, i, j, ms;

void delay_50ms(uint x) // 延时50ms    11.0592晶振
{
    TMOD = 0x01; // 开定时器0，工作方式为1
    TR0 = 1;     // 启动定时器0；
    while (x--)
    {
        TH0 = (65536 - 46080) / 256; // 定时50ms初值的高八位装入TH0
        TL0 = (65536 - 46080) % 256; // 定时50ms初值的低八位装入TL0
        while (!TF0)
            ;    // 等待，直到TF0为1
        TF0 = 0; // 重置溢出标志位
    }
    TR0 = 0; // 停止定时器0；
}
void delay_100us(uint y) // 延时500us
{
    TMOD = 0x10; // 开定时器1，工作方式为1
    TR1 = 1;     // 启动定时器0；
    while (y--)
    {
        TH1 = (65536 - 93) / 256; // 定时500us初值的高八位装入TH0
        TL1 = (65536 - 93) % 256; // 定时1ms初值的低八位装入TL0
        while (!TF1)
            ;    // 等待，直到TF1为1
        TF1 = 0; // 重置溢出标志位
    }
    TR1 = 0; // 停止定时器1；
}

void delay_10us(uint y) // 延时500us
{
    TMOD = 0x10; // 开定时器1，工作方式为1
    TR1 = 1;     // 启动定时器0；
    while (y--)
    {
        TH1 = (65536 - 9) / 256; // 定时500us初值的高八位装入TH0
        TL1 = (65536 - 9) % 256; // 定时1ms初值的低八位装入TL0
        while (!TF1)
            ;    // 等待，直到TF1为1
        TF1 = 0; // 重置溢出标志位
    }
    TR1 = 0; // 停止定时器1；
}

void Servo_PWM(uint angle) // 0到135度
{
    uint x = 50 + ((200 * angle) / (180));
    PWM = 1;
    delay_10us(x);
    PWM = 0;
    delay_10us(2000 - x);
}
void main()
{

    while (1)
    {
        Servo_PWM(0);
        delay_50ms(3);
        Servo_PWM(45);
        delay_50ms(3);
    }
}
