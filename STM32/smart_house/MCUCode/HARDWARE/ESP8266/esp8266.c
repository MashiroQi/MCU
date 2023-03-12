#include "esp8266.h"
#include "string.h"
#include "usart.h"
#include "usart3.h"
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "stdlib.h"
#include "malloc.h"
#include <string.h>

// const u8* wifista_ssid="1503";			//·����SSID��
// const u8* wifista_password="13955354686"; 	//��������

const u8 *wifista_ssid = "1112400";		 // ·����SSID��
const u8 *wifista_password = "q1112400"; // ��������

#define WEATHER_PORTNUM "80"				 // �������Ӷ˿ں�:80
#define WEATHER_SERVERIP "api.seniverse.com" // ����������IP
// #define ESP8266_WIFI_INFO		"AT+CWJAP=\"1503\",\"13955354686\"\r\n"
#define ESP8266_WIFI_INFO "AT+CWJAP=\"1112400\",\"q1112400\"\r\n"

#define ESP8266_ONENET_INFO "AT+CIPSTART=\"TCP\",\"lot.qzhtest.top\",1883\r\n"
#define ESP8266_IP_INFO "AT+CIFSR\r\n"
#define Time_PORTNUM "80"				// �������Ӷ˿ں�:80
#define Time_SERVERIP "quan.suning.com" // ����������IP

u8 *p;
unsigned char esp8266_buf[600];
unsigned int esp8266_cnt = 0, esp8266_cntPre = 0;

void esp8266_start_trans(void)
{
	u8 *p;
	p = mymalloc(SRAMIN, 50); // ����32�ֽ��ڴ棬���ڴ�wifista_ssid��wifista_password
	printf("send:AT\r\n");
	while (esp8266_send_cmd("AT\r\n", "OK", 20))
		; // ���WIFIģ���Ƿ�����
	// ���ù���ģʽ 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
	printf("send:AT+CWMODE=1\r\n");
	esp8266_send_cmd("AT+CWMODE=1\r\n", "OK", 50);
	// Wifiģ������
	printf("send:AT+RST\r\n");
	esp8266_send_cmd("AT+RST\r\n", "OK", 20);
	delay_ms(1000); // ��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);

	// �������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!!
	printf("send:AT+CIPMUX=0\r\n");
	esp8266_send_cmd("AT+CIPMUX=0\r\n", "OK", 20); // 0�������ӣ�1��������

	sprintf((char *)p, "AT+CWJAP=\"%s\",\"%s\"\r\n", wifista_ssid, wifista_password); // �������߲���:ssid,����
	printf("send:AT+CWJAP=\"%s\",\"%s\"\r\n", wifista_ssid, wifista_password);

	while (esp8266_send_cmd(p, "WIFI GOT IP", 300))
		; // ����Ŀ��·����,���һ��IP
	myfree(SRAMIN, p);
}

// ��ESP8266��������
// cmd:���͵������ַ���;ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��;waittime:�ȴ�ʱ��(��λ:10ms)
// ����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����);1,����ʧ��
u8 esp8266_send_cmd(u8 *cmd, u8 *ack, u16 waittime)
{
	return ESP8266_SendCmdTime(cmd, ack, waittime);
	//	u8 res=0;
	//	USART3_RX_STA=0;
	//	u3_printf("%s\r\n",cmd);	//��������
	//	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	//	{
	//		while(--waittime)	//�ȴ�����ʱ
	//		{
	//			delay_ms(10);
	//			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
	//			{
	//				if(esp8266_check_cmd(ack))
	//				{
	//					printf("receive:%s\r\n",(u8*)ack);
	//					break;//�õ���Ч����
	//				}
	//					USART3_RX_STA=0;
	//			}
	//		}
	//		if(waittime==0)res=1;
	//	}
	//	return res;
}
// ESP8266���������,�����յ���Ӧ��
// str:�ڴ���Ӧ����
// ����ֵ:0,û�еõ��ڴ���Ӧ����;����,�ڴ�Ӧ������λ��(str��λ��)
u8 *esp8266_check_cmd(u8 *str)
{
	char *strx = 0;
	if (USART3_RX_STA & 0X8000) // ���յ�һ��������
	{
		USART3_RX_BUF[USART3_RX_STA & 0X7FFF] = 0; // ��ӽ�����
		strx = strstr((const char *)USART3_RX_BUF, (const char *)str);
	}
	return (u8 *)strx;
}

// ��ȡһ��ʵʱ����
// ���أ�0---��ȡ�ɹ���1---��ȡʧ��
u8 get_current_weather(void)
{
	u8 res;
	u8 *WeatherAPI = "GET https://api.seniverse.com/v3/weather/now.json?key=SdhaQd3OweoTJ0uzP&location=wuhu&language=en&unit=c\n\n";
	p = mymalloc(SRAMIN, 40);																	// ����40�ֽ��ڴ�
	sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", WEATHER_SERVERIP, WEATHER_PORTNUM); // ����Ŀ��TCP������
	printf("send:AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", WEATHER_SERVERIP, WEATHER_PORTNUM);
	res = esp8266_send_cmd(p, "OK", 200); // ���ӵ�Ŀ��TCP������
	if (res == 1)
	{
		myfree(SRAMIN, p);
		return 1;
	}
	delay_ms(300);

	printf("send:AT+CIPMODE=1\r\n");
	esp8266_send_cmd("AT+CIPMODE=1\r\n", "OK", 100); // ����ģʽΪ��͸��

	USART3_RX_STA = 0;
	printf("send:AT+CIPSEND\r\n");
	esp8266_send_cmd("AT+CIPSEND\r\n", "OK", 100); // ��ʼ͸��
	printf("start trans...\r\n");

	Usart_SendString(USART3, WeatherAPI, strlen(WeatherAPI));
	// u3_printf("GET https://api.seniverse.com/v3/weather/now.json?key=SdhaQd3OweoTJ0uzP&location=bengbu&language=en&unit=c\n\n");
	delay_ms(20);	   // ��ʱ20ms���ص���ָ��ͳɹ���״̬
	USART3_RX_STA = 0; // ���㴮��3����
	esp8266_cnt = 0;
	delay_ms(1000);
	//	if(USART3_RX_STA&0X8000)		//��ʱ�ٴνӵ�һ�����ݣ�Ϊ����������
	//	{
	//		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
	//	}
	while (ESP8266_WaitRecive() == REV_OK)
	{
		esp8266_buf[esp8266_cnt & 0X7FFF] = 0;
	}
	printf("esp8266_buf=%s\r\n", esp8266_buf);
	memcpy(JSON_BUF, esp8266_buf, sizeof(esp8266_buf));
	atk_8266_quit_trans(); // �˳�͸��

	//	printf("send:AT+CIPCLOSE\r\n");
	//	esp8266_send_cmd("AT+CIPCLOSE\r\n","OK",50);         //�ر�����
	myfree(SRAMIN, p);
	return 0;
}

// ��ȡһ��ʵʱʱ��
// ���أ�0---��ȡ�ɹ���1---��ȡʧ��
u8 get_current_Time(void)
{
	u8 res;
	u8 *TimeAPI = "GET http://quan.suning.com/getSysTime.do\n\n";
	p = mymalloc(SRAMIN, 40); // ����40�ֽ��ڴ�

	sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", Time_SERVERIP, WEATHER_PORTNUM); // ����Ŀ��TCP������
	printf("send:AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", Time_SERVERIP, WEATHER_PORTNUM);
	res = esp8266_send_cmd(p, "OK", 200); // ���ӵ�Ŀ��TCP������
	if (res == 1)
	{
		myfree(SRAMIN, p);

		return 1;
	}
	delay_ms(300);

	printf("send:AT+CIPMODE=1\r\n");
	esp8266_send_cmd("AT+CIPMODE=1\r\n", "OK", 100); // ����ģʽΪ��͸��

	USART3_RX_STA = 0;
	printf("send:AT+CIPSEND\r\n");
	esp8266_send_cmd("AT+CIPSEND\r\n", "OK", 100); // ��ʼ͸��
	printf("start trans...\r\n");

	Usart_SendString(USART3, TimeAPI, strlen(TimeAPI));
	// u3_printf("GET https://api.seniverse.com/v3/weather/now.json?key=SdhaQd3OweoTJ0uzP&location=bengbu&language=en&unit=c\n\n");
	delay_ms(20);	   // ��ʱ20ms���ص���ָ��ͳɹ���״̬
	USART3_RX_STA = 0; // ���㴮��3����
	esp8266_cnt = 0;
	delay_ms(1000);
	//	if(USART3_RX_STA&0X8000)		//��ʱ�ٴνӵ�һ�����ݣ�Ϊ����������
	//	{
	//		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
	//	}
	if (ESP8266_WaitRecive() == REV_OK)
	{
		esp8266_buf[esp8266_cnt & 0X7FFF] = 0;
	}
	printf("esp8266_buf=%s\r\n", esp8266_buf);
	memcpy(JSON_BUF, esp8266_buf, sizeof(esp8266_buf));
	atk_8266_quit_trans(); // �˳�͸��

	printf("send:AT+CIPCLOSE\r\n");
	esp8266_send_cmd("AT+CIPCLOSE\r\n", "OK", 50); // �ر�����
	myfree(SRAMIN, p);
	return 0;
}

// ATK-ESP8266�˳�͸��ģʽ
// ����ֵ:0,�˳��ɹ�;
//        1,�˳�ʧ��
u8 atk_8266_quit_trans(void)
{
	while ((USART3->SR & 0X40) == 0)
		; // �ȴ����Ϳ�
	USART3->DR = '+';
	delay_ms(15); // ���ڴ�����֡ʱ��(10ms)
	while ((USART3->SR & 0X40) == 0)
		; // �ȴ����Ϳ�
	USART3->DR = '+';
	delay_ms(15); // ���ڴ�����֡ʱ��(10ms)
	while ((USART3->SR & 0X40) == 0)
		; // �ȴ����Ϳ�
	USART3->DR = '+';
	delay_ms(500);							 // �ȴ�500ms
	return esp8266_send_cmd("AT", "OK", 20); // �˳�͸���ж�.
}

//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;
}

//==========================================================
//	�������ƣ�	ESP8266_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if (esp8266_cnt == 0) // ������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;

	if (esp8266_cnt == esp8266_cntPre) // �����һ�ε�ֵ�������ͬ����˵���������
	{
		esp8266_cnt = 0; // ��0���ռ���

		return REV_OK; // ���ؽ�����ɱ�־
	}

	esp8266_cntPre = esp8266_cnt; // ��Ϊ��ͬ

	return REV_WAIT; // ���ؽ���δ��ɱ�־
}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	//	return esp8266_send_cmd(cmd,res,200);
	unsigned char timeOut = 200;

	Usart_SendString(USART3, (unsigned char *)cmd, strlen((const char *)cmd));

	while (timeOut--)
	{
		if (ESP8266_WaitRecive() == REV_OK) // ����յ�����
		{
			if (strstr((const char *)esp8266_buf, res) != NULL) // ����������ؼ���
			{
				ESP8266_Clear(); // ��ջ���

				return 0;
			}
		}

		delay_ms(10);
	}

	return 1;
}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����
//==========================================================
_Bool ESP8266_SendCmdTime(char *cmd, char *res, u16 time)
{
	//	return esp8266_send_cmd(cmd,res,200);
	unsigned char timeOut = time;

	Usart_SendString(USART3, (unsigned char *)cmd, strlen((const char *)cmd));

	while (timeOut--)
	{
		if (ESP8266_WaitRecive() == REV_OK) // ����յ�����
		{
			if (strstr((const char *)esp8266_buf, res) != NULL) // ����������ؼ���
			{
				ESP8266_Clear(); // ��ջ���

				return 0;
			}
		}

		delay_ms(10);
	}

	return 1;
}
//==========================================================
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];

	ESP8266_Clear();						   // ��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len); // ��������
	if (!ESP8266_SendCmd(cmdBuf, ">"))		   // �յ���>��ʱ���Է�������
	{
		Usart_SendString(USART3, data, len); // �����豸������������
	}
}

//==========================================================
//	�������ƣ�	ESP8266_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;

	do
	{
		if (ESP8266_WaitRecive() == REV_OK) // ����������
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,"); // ������IPD��ͷ
			if (ptrIPD == NULL)							  // ���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				// UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':'); // �ҵ�':'
				if (ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
			}
		}

		delay_ms(5);
		timeOut--; // ��ʱ�ȴ�
	} while (timeOut > 0);

	return NULL; // ��ʱ��δ�ҵ������ؿ�ָ��
}

//==========================================================
//	�������ƣ�	ESP8266_Init
//
//	�������ܣ�	��ʼ��ESP8266
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����
//==========================================================
void ESP8266_Init(void)
{
	u8 count = 0;
	ESP8266_Clear();
	printf("send:AT+RST\r\n");
	ESP8266_SendCmd("AT+RST\r\n", "OK");

	UsartPrintf(USART_DEBUG, "1. AT\r\n");
	while (count > 10 || ESP8266_SendCmd("AT\r\n", "OK"))
	{
		count++;
		delay_ms(500);
	}
	printf("send:AT+RST\r\n");
	esp8266_send_cmd("AT+RST", "OK", 20);

	UsartPrintf(USART_DEBUG, "2. CWMODE\r\n");
	while (ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		delay_ms(500);

	UsartPrintf(USART_DEBUG, "3. AT+CWDHCP\r\n");
	while (ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
		delay_ms(500);

	UsartPrintf(USART_DEBUG, "4. CWJAP\r\n");
	while (ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		delay_ms(500);

	UsartPrintf(USART_DEBUG, "5. CIPSTART\r\n");
	while (ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);

	UsartPrintf(USART_DEBUG, "6. ESP8266 Init OK\r\n");
}

//==========================================================
//	�������ƣ�	USART3_IRQHandler

//	�������ܣ�	����2�շ��ж�

//	��ڲ�����	��

//	���ز�����	��

//	˵����
//==========================================================
void USART3_IRQHandler(void)
{

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // �����ж�
	{
		if (esp8266_cnt >= sizeof(esp8266_buf))
			esp8266_cnt = 0; // ��ֹ���ڱ�ˢ��
		esp8266_buf[esp8266_cnt++] = USART3->DR;

		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
}
