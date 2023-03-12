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

// 处理语言识别模块发送数据
void Process_Voice_recognition(u8 cmd)
{
	printf("cmd=%d\r\n", cmd);
	if (cmd == LED_ON)
	{
		printf("开灯");
		LED1 = 0;
		if (!LED1)
		{
			ai_load_picfile("0:/PICTURE/buttomOn.jpg", 245, 285, 40, 40, 1); // 显示图片
		}
		else
		{
			ai_load_picfile("0:/PICTURE/buttomOff.jpg", 245, 285, 40, 40, 1); // 显示图片
		}
	}
	if (cmd == LED_OFF)
	{
		LED1 = 1;
		printf("关灯");
		if (!LED1)
		{
			ai_load_picfile("0:/PICTURE/buttomOn.jpg", 245, 285, 40, 40, 1); // 显示图片
		}
		else
		{
			ai_load_picfile("0:/PICTURE/buttomOff.jpg", 245, 285, 40, 40, 1); // 显示图片
		}
	}
	if (cmd == BEEP_ON)
	{
		BEEP = 1;
		if (BEEP)
		{
			ai_load_picfile("0:/PICTURE/buttomOn.jpg", 90, 380, 40, 40, 1); // 显示图片
		}
		else
		{
			ai_load_picfile("0:/PICTURE/buttomOff.jpg", 90, 380, 40, 40, 1); // 显示图片
		}
	}
	if (cmd == BEEP_OFF)
	{
		BEEP = 0;
		if (BEEP)
		{
			ai_load_picfile("0:/PICTURE/buttomOn.jpg", 90, 380, 40, 40, 1); // 显示图片
		}
		else
		{
			ai_load_picfile("0:/PICTURE/buttomOff.jpg", 90, 380, 40, 40, 1); // 显示图片
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
		// Background(page); // 加载页面背景
	}
	if (cmd == MUSIC_OFF)
	{
		page = 1;
		// Background(page); // 加载页面背景
	}
}
void USART2_IRQHandler(void)
{
	u8 res;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // 接收到数据
	{
		res = USART_ReceiveData(USART2);
		Process_Voice_recognition(res);
	}
}

// 初始化IO 串口2
// pclk1:PCLK1时钟频率(Mhz)
// bound:波特率
void usart2_init(u32 bound)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // 串口2时钟使能

	USART_DeInit(USART2); // 复位串口2

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  // 初始化PA3

	USART_InitStructure.USART_BaudRate = bound;										// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx;									// 收发模式

	USART_Init(USART2, &USART_InitStructure); // 初始化串口	3

	USART_Cmd(USART2, ENABLE); // 使能串口

	// 使能接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 开启中断

	// 设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器

	//	TIM7_Int_Init(1000-1,7200-1);		//10ms中断
	//	USART2_RX_STA=0;		//清零
	//	TIM_Cmd(TIM7,DISABLE);			//关闭定时器7

	//	TIM_Cmd(TIM7,ENABLE);//使能定时器7
}
