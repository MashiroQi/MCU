#ifndef __ESP8266_H
#define __ESP8266_H
#include "sys.h"
#include "stdio.h"	
#include "usart3.h"
//用户配置AT指令
extern char restart[];
extern char cwmode[];
extern char cwlap[];
extern char cwjap[];
extern char cifsr[];
extern char cipmux[];
extern char cipstart[];
extern char cipsend[];
extern char cipserver[];
extern char cwlif[];
extern char cipstatus[];
extern char cipsto[];
extern char cipmode[];
extern char test[];

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志


//函数

u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* esp8266_send_data(u8 *cmd,u16 waittime);
u8* esp8266_check_cmd(u8 *str);
void esp8266_start_trans(void);
u8 esp8266_quit_trans(void);
u8 get_current_weather(void);
u8 cloes_weather(void);
u8 get_weather(void);
u8 atk_8266_quit_trans(void);

void ESP8266_Init(void);
void ESP8266_Clear(void);
void ESP8266_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);
_Bool ESP8266_SendCmd(char *cmd, char *res);
_Bool ESP8266_SendCmdTime(char *cmd, char *res,u16 time);
_Bool ESP8266_WaitRecive(void);

#endif
