#include <REG52.H>
#include <stdio.h>
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
sbit SCL = P1 ^ 5; // IIC时钟引脚定义
sbit SDA = P1 ^ 4; // IIC数据引脚定义
void main()
{
}