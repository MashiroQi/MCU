#include "MQ2.h"
#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h"

//��������
void MQ2_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOG9
}
//��ȡ��������
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