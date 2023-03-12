#include "usart3.h"
#include <string.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "usmart.h"
#include "malloc.h"
#include "sdio_sdcard.h"
#include "w25qxx.h"
#include "ff.h"
#include "exfuns.h"
#include "text.h"
#include "dht11.h"
#include "esp8266.h"
#include "cJSON.h"
#include "page.h"
#include "touch.h"
#include "onenet.h"
#include "beep.h"
#include "MQ2.h"
#include "relay.h"
#include "timer.h"
#include "page.h"
void HardWare_Init(void);

int main(void)
{
	HardWare_Init(); // 硬件初始化
	// TP_Adjust();					  // 触摸屏校准
	TIM_SetCompare2(TIM3, 500); // 舵机复位
	// LCD_Clear(WHITE);			// 清空屏幕
	BackgroundInit(); // 初始化背景

	while (1)
	{
		PageSelect(); // 页面选择
	}
}

void HardWare_Init()
{
	u8 key;											// 键值
	delay_init();									// 延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);								// 串口初始化为115200
	usart3_init(115200);							// 串口3初始化
	usart2_init(115200);							// 串口2初始化
	RELAY_Init();									// 继电器（风扇）初始化
	usmart_dev.init(72);							// 初始化USMART
	LED_Init();										// 初始化与LED连接的硬件接口
	BEEP_Init();									// 蜂鸣器初始化
	MQ2_Init();										// 烟雾传感器初始化
	KEY_Init();										// 初始化按键
	LCD_Init();										// 初始化LCD
	tp_dev.init();									// 触摸屏初始化
	VS_Init();										// 初始化VS1053
	W25QXX_Init();									// 初始化W25Q128
	my_mem_init(SRAMIN);							// 初始化内部内存池
	exfuns_init();									// 为fatfs相关变量申请内存
	f_mount(fs[0], "0:", 1);						// 挂载SD卡
	f_mount(fs[1], "1:", 1);						// 挂载FLASH.
	piclib_init();									// 绘图初始化
	TIM3_PWM_Init(22499, 63);						// 不分频。PWM频率=72000000/900=80Khz
	TIM_SetCompare2(TIM3, 570);

	while (font_init()) // 检查字库
	{
		LCD_Clear(WHITE);  // 清屏
		POINT_COLOR = RED; // 设置字体为红色
		LCD_ShowString(30, 50, 200, 16, 16, "WarShip STM32");
		while (SD_Init()) // 检测SD卡
		{
			LCD_ShowString(30, 70, 200, 16, 16, "SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30, 70, 200 + 30, 70 + 16, WHITE);
			delay_ms(200);
		}
		LCD_ShowString(30, 70, 200, 16, 16, "SD Card OK");
		LCD_ShowString(30, 90, 200, 16, 16, "Font Updating...");
		key = update_font(20, 110, 16, "0:"); // 更新字库
		while (key)							  // 更新失败
		{
			LCD_ShowString(30, 110, 200, 16, 16, "Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20, 110, 200 + 20, 110 + 16, WHITE);
			delay_ms(200);
		}
		LCD_ShowString(30, 110, 200, 16, 16, "Font Update Success!   ");
		LCD_Clear(WHITE); // 清屏
	}

	UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");
}
