#include <REGX51.H>
#include "stdio.h"

sfr ADC_CONTR = 0xbc;
sfr ADC_RES = 0xbe;
sfr ADC_RESL = 0x97;

void Serial_init()
{
    TMOD |= 0X20;
    TH1 = 0Xfd;
    TL1 = 0Xfd; // 设置波特率9600
    TR1 = 1;    // 开定时器1中断
    SM0 = 0;    // 工作方式1
    SM1 = 1;
    ES = 1; // 开始串口中断
    EA = 1; // 开总中断
    TI = 1;
}
unsigned int ADC(unsigned char channel)
{
    ADC_CONTR = 0xe8 | (channel & 0x7);
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    while (ADC_CONTR & 0x10)
        ;
    return (unsigned int)((unsigned int)(ADC_RES << 2) | (unsigned int)(ADC_RESL));
}

int main() // 主函数
{
    Serial_init(); // 串口初始化
    while (1)
    {
    }
}