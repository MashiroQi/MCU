#include "MQ2.h"
#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h"

//烟雾传感器
void MQ2_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
 	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOG9
}
//读取烟雾传感器
u8 MQ2_Scan()
{	 
	if(MQ2==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}