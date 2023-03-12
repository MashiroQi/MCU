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

// const u8* wifista_ssid="1503";			//路由器SSID号
// const u8* wifista_password="13955354686"; 	//连接密码

const u8 *wifista_ssid = "1112400";		 // 路由器SSID号
const u8 *wifista_password = "q1112400"; // 连接密码

#define WEATHER_PORTNUM "80"				 // 天气连接端口号:80
#define WEATHER_SERVERIP "api.seniverse.com" // 天气服务器IP
// #define ESP8266_WIFI_INFO		"AT+CWJAP=\"1503\",\"13955354686\"\r\n"
#define ESP8266_WIFI_INFO "AT+CWJAP=\"1112400\",\"q1112400\"\r\n"

#define ESP8266_ONENET_INFO "AT+CIPSTART=\"TCP\",\"lot.qzhtest.top\",1883\r\n"
#define ESP8266_IP_INFO "AT+CIFSR\r\n"
#define Time_PORTNUM "80"				// 天气连接端口号:80
#define Time_SERVERIP "quan.suning.com" // 天气服务器IP

u8 *p;
unsigned char esp8266_buf[600];
unsigned int esp8266_cnt = 0, esp8266_cntPre = 0;

void esp8266_start_trans(void)
{
	u8 *p;
	p = mymalloc(SRAMIN, 50); // 申请32字节内存，用于存wifista_ssid，wifista_password
	printf("send:AT\r\n");
	while (esp8266_send_cmd("AT\r\n", "OK", 20))
		; // 检查WIFI模块是否在线
	// 设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
	printf("send:AT+CWMODE=1\r\n");
	esp8266_send_cmd("AT+CWMODE=1\r\n", "OK", 50);
	// Wifi模块重启
	printf("send:AT+RST\r\n");
	esp8266_send_cmd("AT+RST\r\n", "OK", 20);
	delay_ms(1000); // 延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);

	// 设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!!
	printf("send:AT+CIPMUX=0\r\n");
	esp8266_send_cmd("AT+CIPMUX=0\r\n", "OK", 20); // 0：单连接，1：多连接

	sprintf((char *)p, "AT+CWJAP=\"%s\",\"%s\"\r\n", wifista_ssid, wifista_password); // 设置无线参数:ssid,密码
	printf("send:AT+CWJAP=\"%s\",\"%s\"\r\n", wifista_ssid, wifista_password);

	while (esp8266_send_cmd(p, "WIFI GOT IP", 300))
		; // 连接目标路由器,并且获得IP
	myfree(SRAMIN, p);
}

// 向ESP8266发送命令
// cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
// 返回值:0,发送成功(得到了期待的应答结果);1,发送失败
u8 esp8266_send_cmd(u8 *cmd, u8 *ack, u16 waittime)
{
	return ESP8266_SendCmdTime(cmd, ack, waittime);
	//	u8 res=0;
	//	USART3_RX_STA=0;
	//	u3_printf("%s\r\n",cmd);	//发送命令
	//	if(ack&&waittime)		//需要等待应答
	//	{
	//		while(--waittime)	//等待倒计时
	//		{
	//			delay_ms(10);
	//			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
	//			{
	//				if(esp8266_check_cmd(ack))
	//				{
	//					printf("receive:%s\r\n",(u8*)ack);
	//					break;//得到有效数据
	//				}
	//					USART3_RX_STA=0;
	//			}
	//		}
	//		if(waittime==0)res=1;
	//	}
	//	return res;
}
// ESP8266发送命令后,检测接收到的应答
// str:期待的应答结果
// 返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8 *esp8266_check_cmd(u8 *str)
{
	char *strx = 0;
	if (USART3_RX_STA & 0X8000) // 接收到一次数据了
	{
		USART3_RX_BUF[USART3_RX_STA & 0X7FFF] = 0; // 添加结束符
		strx = strstr((const char *)USART3_RX_BUF, (const char *)str);
	}
	return (u8 *)strx;
}

// 获取一次实时天气
// 返回：0---获取成功，1---获取失败
u8 get_current_weather(void)
{
	u8 res;
	u8 *WeatherAPI = "GET https://api.seniverse.com/v3/weather/now.json?key=SdhaQd3OweoTJ0uzP&location=wuhu&language=en&unit=c\n\n";
	p = mymalloc(SRAMIN, 40);																	// 申请40字节内存
	sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", WEATHER_SERVERIP, WEATHER_PORTNUM); // 配置目标TCP服务器
	printf("send:AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", WEATHER_SERVERIP, WEATHER_PORTNUM);
	res = esp8266_send_cmd(p, "OK", 200); // 连接到目标TCP服务器
	if (res == 1)
	{
		myfree(SRAMIN, p);
		return 1;
	}
	delay_ms(300);

	printf("send:AT+CIPMODE=1\r\n");
	esp8266_send_cmd("AT+CIPMODE=1\r\n", "OK", 100); // 传输模式为：透传

	USART3_RX_STA = 0;
	printf("send:AT+CIPSEND\r\n");
	esp8266_send_cmd("AT+CIPSEND\r\n", "OK", 100); // 开始透传
	printf("start trans...\r\n");

	Usart_SendString(USART3, WeatherAPI, strlen(WeatherAPI));
	// u3_printf("GET https://api.seniverse.com/v3/weather/now.json?key=SdhaQd3OweoTJ0uzP&location=bengbu&language=en&unit=c\n\n");
	delay_ms(20);	   // 延时20ms返回的是指令发送成功的状态
	USART3_RX_STA = 0; // 清零串口3数据
	esp8266_cnt = 0;
	delay_ms(1000);
	//	if(USART3_RX_STA&0X8000)		//此时再次接到一次数据，为天气的数据
	//	{
	//		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
	//	}
	while (ESP8266_WaitRecive() == REV_OK)
	{
		esp8266_buf[esp8266_cnt & 0X7FFF] = 0;
	}
	printf("esp8266_buf=%s\r\n", esp8266_buf);
	memcpy(JSON_BUF, esp8266_buf, sizeof(esp8266_buf));
	atk_8266_quit_trans(); // 退出透传

	//	printf("send:AT+CIPCLOSE\r\n");
	//	esp8266_send_cmd("AT+CIPCLOSE\r\n","OK",50);         //关闭连接
	myfree(SRAMIN, p);
	return 0;
}

// 获取一次实时时间
// 返回：0---获取成功，1---获取失败
u8 get_current_Time(void)
{
	u8 res;
	u8 *TimeAPI = "GET http://quan.suning.com/getSysTime.do\n\n";
	p = mymalloc(SRAMIN, 40); // 申请40字节内存

	sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", Time_SERVERIP, WEATHER_PORTNUM); // 配置目标TCP服务器
	printf("send:AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", Time_SERVERIP, WEATHER_PORTNUM);
	res = esp8266_send_cmd(p, "OK", 200); // 连接到目标TCP服务器
	if (res == 1)
	{
		myfree(SRAMIN, p);

		return 1;
	}
	delay_ms(300);

	printf("send:AT+CIPMODE=1\r\n");
	esp8266_send_cmd("AT+CIPMODE=1\r\n", "OK", 100); // 传输模式为：透传

	USART3_RX_STA = 0;
	printf("send:AT+CIPSEND\r\n");
	esp8266_send_cmd("AT+CIPSEND\r\n", "OK", 100); // 开始透传
	printf("start trans...\r\n");

	Usart_SendString(USART3, TimeAPI, strlen(TimeAPI));
	// u3_printf("GET https://api.seniverse.com/v3/weather/now.json?key=SdhaQd3OweoTJ0uzP&location=bengbu&language=en&unit=c\n\n");
	delay_ms(20);	   // 延时20ms返回的是指令发送成功的状态
	USART3_RX_STA = 0; // 清零串口3数据
	esp8266_cnt = 0;
	delay_ms(1000);
	//	if(USART3_RX_STA&0X8000)		//此时再次接到一次数据，为天气的数据
	//	{
	//		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
	//	}
	if (ESP8266_WaitRecive() == REV_OK)
	{
		esp8266_buf[esp8266_cnt & 0X7FFF] = 0;
	}
	printf("esp8266_buf=%s\r\n", esp8266_buf);
	memcpy(JSON_BUF, esp8266_buf, sizeof(esp8266_buf));
	atk_8266_quit_trans(); // 退出透传

	printf("send:AT+CIPCLOSE\r\n");
	esp8266_send_cmd("AT+CIPCLOSE\r\n", "OK", 50); // 关闭连接
	myfree(SRAMIN, p);
	return 0;
}

// ATK-ESP8266退出透传模式
// 返回值:0,退出成功;
//        1,退出失败
u8 atk_8266_quit_trans(void)
{
	while ((USART3->SR & 0X40) == 0)
		; // 等待发送空
	USART3->DR = '+';
	delay_ms(15); // 大于串口组帧时间(10ms)
	while ((USART3->SR & 0X40) == 0)
		; // 等待发送空
	USART3->DR = '+';
	delay_ms(15); // 大于串口组帧时间(10ms)
	while ((USART3->SR & 0X40) == 0)
		; // 等待发送空
	USART3->DR = '+';
	delay_ms(500);							 // 等待500ms
	return esp8266_send_cmd("AT", "OK", 20); // 退出透传判断.
}

//==========================================================
//	函数名称：	ESP8266_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;
}

//==========================================================
//	函数名称：	ESP8266_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if (esp8266_cnt == 0) // 如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;

	if (esp8266_cnt == esp8266_cntPre) // 如果上一次的值和这次相同，则说明接收完毕
	{
		esp8266_cnt = 0; // 清0接收计数

		return REV_OK; // 返回接收完成标志
	}

	esp8266_cntPre = esp8266_cnt; // 置为相同

	return REV_WAIT; // 返回接收未完成标志
}

//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	//	return esp8266_send_cmd(cmd,res,200);
	unsigned char timeOut = 200;

	Usart_SendString(USART3, (unsigned char *)cmd, strlen((const char *)cmd));

	while (timeOut--)
	{
		if (ESP8266_WaitRecive() == REV_OK) // 如果收到数据
		{
			if (strstr((const char *)esp8266_buf, res) != NULL) // 如果检索到关键词
			{
				ESP8266_Clear(); // 清空缓存

				return 0;
			}
		}

		delay_ms(10);
	}

	return 1;
}

//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：
//==========================================================
_Bool ESP8266_SendCmdTime(char *cmd, char *res, u16 time)
{
	//	return esp8266_send_cmd(cmd,res,200);
	unsigned char timeOut = time;

	Usart_SendString(USART3, (unsigned char *)cmd, strlen((const char *)cmd));

	while (timeOut--)
	{
		if (ESP8266_WaitRecive() == REV_OK) // 如果收到数据
		{
			if (strstr((const char *)esp8266_buf, res) != NULL) // 如果检索到关键词
			{
				ESP8266_Clear(); // 清空缓存

				return 0;
			}
		}

		delay_ms(10);
	}

	return 1;
}
//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];

	ESP8266_Clear();						   // 清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len); // 发送命令
	if (!ESP8266_SendCmd(cmdBuf, ">"))		   // 收到‘>’时可以发送数据
	{
		Usart_SendString(USART3, data, len); // 发送设备连接请求数据
	}
}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;

	do
	{
		if (ESP8266_WaitRecive() == REV_OK) // 如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,"); // 搜索“IPD”头
			if (ptrIPD == NULL)							  // 如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				// UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':'); // 找到':'
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
		timeOut--; // 延时等待
	} while (timeOut > 0);

	return NULL; // 超时还未找到，返回空指针
}

//==========================================================
//	函数名称：	ESP8266_Init
//
//	函数功能：	初始化ESP8266
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：
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
//	函数名称：	USART3_IRQHandler

//	函数功能：	串口2收发中断

//	入口参数：	无

//	返回参数：	无

//	说明：
//==========================================================
void USART3_IRQHandler(void)
{

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // 接收中断
	{
		if (esp8266_cnt >= sizeof(esp8266_buf))
			esp8266_cnt = 0; // 防止串口被刷爆
		esp8266_buf[esp8266_cnt++] = USART3->DR;

		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
}
