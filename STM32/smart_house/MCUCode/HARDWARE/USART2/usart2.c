#include "delay.h"
#include "usart2.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "timer.h"
#include "string.h"
#include "led.h"
#include "beep.h"
#include "relay.h"
#include "piclib.h"
#include "page.h"

enum CMD
{
	LED_ON,
	LED_OFF,
	BEEP_ON,
	BEEP_OFF,
	RELAY_ON,
	RELAY_OFF,
	SERVO_ON,
	SERVO_OFF,
	MUSIC_ON,
	MUSIC_OFF
};

// ��������ʶ��ģ�鷢������
void Process_Voice_recognition(u8 cmd)
{
	printf("cmd=%d\r\n", cmd);
	if (cmd == LED_ON)
	{
		printf("����");
		LED1 = 0;
		if (!LED1)
		{
			ai_load_picfile("0:/PICTURE/buttomOn.jpg", 245, 285, 40, 40, 1); // ��ʾͼƬ
		}
		else
		{
			ai_load_picfile("0:/PICTURE/buttomOff.jpg", 245, 285, 40, 40, 1); // ��ʾͼƬ
		}
	}
	if (cmd == LED_OFF)
	{
		LED1 = 1;
		printf("�ص�");
		if (!LED1)
		{
			ai_load_picfile("0:/PICTURE/buttomOn.jpg", 245, 285, 40, 40, 1); // ��ʾͼƬ
		}
		else
		{
			ai_load_picfile("0:/PICTURE/buttomOff.jpg", 245, 285, 40, 40, 1); // ��ʾͼƬ
		}
	}
	if (cmd == BEEP_ON)
	{
		BEEP = 1;
		if (BEEP)
		{
			ai_load_picfile("0:/PICTURE/buttomOn.jpg", 90, 380, 40, 40, 1); // ��ʾͼƬ
		}
		else
		{
			ai_load_picfile("0:/PICTURE/buttomOff.jpg", 90, 380, 40, 40, 1); // ��ʾͼƬ
		}
	}
	if (cmd == BEEP_OFF)
	{
		BEEP = 0;
		if (BEEP)
		{
			ai_load_picfile("0:/PICTURE/buttomOn.jpg", 90, 380, 40, 40, 1); // ��ʾͼƬ
		}
		else
		{
			ai_load_picfile("0:/PICTURE/buttomOff.jpg", 90, 380, 40, 40, 1); // ��ʾͼƬ
		}
	}
	if (cmd == RELAY_ON)
	{
		RELAY = 0;
	}
	if (cmd == RELAY_OFF)
	{
		RELAY = 1;
	}
	if (cmd == SERVO_ON)
	{
		pwm_servo = 570;
		TIM_SetCompare2(TIM3, 570);
	}
	if (cmd == SERVO_OFF)
	{
		pwm_servo = 2200;
		TIM_SetCompare2(TIM3, 2200);
	}
	if (cmd == MUSIC_ON)
	{
		page = 2;
		// Background(page); // ����ҳ�汳��
	}
	if (cmd == MUSIC_OFF)
	{
		page = 1;
		// Background(page); // ����ҳ�汳��
	}
}
void USART2_IRQHandler(void)
{
	u8 res;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // ���յ�����
	{
		res = USART_ReceiveData(USART2);
		Process_Voice_recognition(res);
	}
}

// ��ʼ��IO ����2
// pclk1:PCLK1ʱ��Ƶ��(Mhz)
// bound:������
void usart2_init(u32 bound)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // ����2ʱ��ʹ��

	USART_DeInit(USART2); // ��λ����2

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  // ��ʼ��PA3

	USART_InitStructure.USART_BaudRate = bound;										// ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx;									// �շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); // ��ʼ������	3

	USART_Cmd(USART2, ENABLE); // ʹ�ܴ���

	// ʹ�ܽ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // �����ж�

	// �����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // ��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  // ����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//	TIM7_Int_Init(1000-1,7200-1);		//10ms�ж�
	//	USART2_RX_STA=0;		//����
	//	TIM_Cmd(TIM7,DISABLE);			//�رն�ʱ��7

	//	TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
}
