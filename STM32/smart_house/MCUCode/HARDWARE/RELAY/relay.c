#include "stm32f10x.h"
#include "relay.h"
#include "sys.h" 


void RELAY_Init()
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOG, &GPIO_InitStructure);					
	RELAY=1;
}
