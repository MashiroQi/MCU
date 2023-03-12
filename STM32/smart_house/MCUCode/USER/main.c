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
	HardWare_Init(); // Ӳ����ʼ��
	// TP_Adjust();					  // ������У׼
	TIM_SetCompare2(TIM3, 500); // �����λ
	// LCD_Clear(WHITE);			// �����Ļ
	BackgroundInit(); // ��ʼ������

	while (1)
	{
		PageSelect(); // ҳ��ѡ��
	}
}

void HardWare_Init()
{
	u8 key;											// ��ֵ
	delay_init();									// ��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);								// ���ڳ�ʼ��Ϊ115200
	usart3_init(115200);							// ����3��ʼ��
	usart2_init(115200);							// ����2��ʼ��
	RELAY_Init();									// �̵��������ȣ���ʼ��
	usmart_dev.init(72);							// ��ʼ��USMART
	LED_Init();										// ��ʼ����LED���ӵ�Ӳ���ӿ�
	BEEP_Init();									// ��������ʼ��
	MQ2_Init();										// ����������ʼ��
	KEY_Init();										// ��ʼ������
	LCD_Init();										// ��ʼ��LCD
	tp_dev.init();									// ��������ʼ��
	VS_Init();										// ��ʼ��VS1053
	W25QXX_Init();									// ��ʼ��W25Q128
	my_mem_init(SRAMIN);							// ��ʼ���ڲ��ڴ��
	exfuns_init();									// Ϊfatfs��ر��������ڴ�
	f_mount(fs[0], "0:", 1);						// ����SD��
	f_mount(fs[1], "1:", 1);						// ����FLASH.
	piclib_init();									// ��ͼ��ʼ��
	TIM3_PWM_Init(22499, 63);						// ����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM_SetCompare2(TIM3, 570);

	while (font_init()) // ����ֿ�
	{
		LCD_Clear(WHITE);  // ����
		POINT_COLOR = RED; // ��������Ϊ��ɫ
		LCD_ShowString(30, 50, 200, 16, 16, "WarShip STM32");
		while (SD_Init()) // ���SD��
		{
			LCD_ShowString(30, 70, 200, 16, 16, "SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30, 70, 200 + 30, 70 + 16, WHITE);
			delay_ms(200);
		}
		LCD_ShowString(30, 70, 200, 16, 16, "SD Card OK");
		LCD_ShowString(30, 90, 200, 16, 16, "Font Updating...");
		key = update_font(20, 110, 16, "0:"); // �����ֿ�
		while (key)							  // ����ʧ��
		{
			LCD_ShowString(30, 110, 200, 16, 16, "Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20, 110, 200 + 20, 110 + 16, WHITE);
			delay_ms(200);
		}
		LCD_ShowString(30, 110, 200, 16, 16, "Font Update Success!   ");
		LCD_Clear(WHITE); // ����
	}

	UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");
}
