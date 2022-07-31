#ifndef PUBLIC_H
#define PUBLIC_H
#include <regx52.h>
#include <intrins.h>


typedef unsigned char uchar;
typedef unsigned int uint;

void delay(uchar x)
{
	while(x--);
}
void delay_ms(uchar x)
{
	uchar i,j;
	for(i=x;i>0;i--)
		for(j=110;j>0;j--);
}

#endif