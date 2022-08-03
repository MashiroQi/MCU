#include "stm32f10x.h"                 

int main()
{
	GPIO_InitTypeDef LED;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	
	LED.GPIO_Mode = GPIO_Mode_Out_PP;
	LED.GPIO_Pin = GPIO_Pin_13;
	LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&LED);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	while(1)
	{
	
	}
}
