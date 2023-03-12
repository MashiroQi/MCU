#include "lcd.h"
#include "text.h"
#include "cJSON.h"
#include "esp8266.h"
#include "page.h"
#include "dht11.h"
#include "stddef.h"
#include "string.h"
#include "delay.h"
#include "piclib.h"
#include "touch.h"
#include "led.h"
#include "beep.h"
#include "mp3player.h"
#include "vs10xx.h"
#include "MQ2.h"
#include "relay.h"

u16 totpicnum; // 图片文件总数
u8 res;
DIR picdir;			 // 图片目录
FILINFO picfileinfo; // 文件信息
u8 *fn;				 // 长文件名
u8 *pname;			 // 带路径的文件名
u16 curindex;		 // 图片当前索引
u8 temp1;
u16 *picindextbl; // 图片索引表

u8 PUB_BUF[250];
u8 temp, humi;
const char *topics[] = {"/home/sub"};
unsigned short timeCount = 0; // 发送间隔变量
u8 *dataPtr = NULL;
u8 page = 1;
u8 previousPage = 0;
u8 weatherText[10];

int pwm_servo = 570; // min570 max2800

// 触摸检测
void TouchScan()
{
	tp_dev.scan(0); // 扫描触摸屏
	if (tp_dev.sta & TP_PRES_DOWN)
	{
		if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height)
		{
			//			printf("x=%d,y=%d",tp_dev.x,tp_dev.y);
			//			if(tp_dev.x[0]>0)
			//			TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图
		}
	}
}

// mqtt接口初始化
void MqttInit()
{
	ESP8266_Init();			 // esp8266
	while (OneNet_DevLink()) // 接入mqtt服务器
		delay_ms(1000);
	OneNet_Subscribe(topics, 1); // 订阅主题
}

// 与mqtt服务器交换数据
void MqttUpdate()
{
	if (++timeCount >= 200) // 发送间隔5s
	{
		// UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
		ShowTempHumi();
		sprintf(PUB_BUF, "{\"Temp\":%d ,\"Humi\":%d,\"Smoke\":%d,\"Servo\":%d}", temp, humi, MQ2_Scan(), pwm_servo > 1000 ? 0 : 1);
		// 发送温湿度
		OneNet_Publish("/home/pub", PUB_BUF);
		timeCount = 0;
		ESP8266_Clear();
	}
	// 等待接受数据
	dataPtr = ESP8266_GetIPD(10);
	if (dataPtr != NULL)
	{
		printf("%s", dataPtr);
		OneNet_RevPro(dataPtr); // 处理接收数据
	}
}

// 显示天气
void Show_Weather()
{
	cJSON *root = NULL;
	cJSON *root1 = NULL;
	cJSON *root2 = NULL;
	if (strlen(weatherText) < 2)
	{
		get_current_weather();
		//		printf("JSON_BUF=%s",JSON_BUF);
		// 解析天气JSON数据
		root = cJSON_Parse(JSON_BUF);
		root1 = cJSON_GetObjectItem(root, "results");
		root1 = cJSON_GetArrayItem(root1, 0);
		root1 = cJSON_GetObjectItem(root1, "now");
		root2 = cJSON_GetObjectItem(root1, "text");
		//		if(root2->child != NULL)
		// 		printf("\n\n\ntext=%s\n", cJSON_Print(root2));
		strcpy(weatherText, root2->valuestring);
	}
	Show_Str(80, 290, 200, 24, weatherText, 24, 1);
	// 获取JSON中温度
	//	root2 = cJSON_GetObjectItem(root1, "temperature");
	//	if(root2->child != NULL)
	//		printf("temperature=%s\n", cJSON_Print(root2));
	//	Show_Str(90,290,200,16,root2->valuestring,16,1);
	cJSON_Delete(root);
}

// 显示温度湿度
void ShowTempHumi()
{
	do
	{
		DHT11_Read_Data(&temp, &humi);
		LCD_ShowNum(90, 190, temp, 2, 24);
		LCD_ShowNum(250, 190, humi, 2, 24);
	} while (humi == 0);
}

// 首页初始化
void PagehomeInit()
{

	POINT_COLOR = WHITE;
	Show_Str_Mid(45, 70, "智能家居", 24, 500);
	POINT_COLOR = BLACK;
	Show_Str(90, 190, 200, 24, "  ℃", 24, 1);
	Show_Str(250, 190, 120, 24, "  %", 24, 1);
	ESP8266_Clear();
	esp8266_start_trans();
	POINT_COLOR = RED;
	while (DHT11_Init())
		;
	ShowTempHumi();
	Show_Weather();
	MqttInit();
}

// 首页
void Pagehome()
{
	TouchScan(); // 扫描触摸屏
	if (tp_dev.sta & TP_PRES_DOWN)
	{
		if (tp_dev.x[0] < (lcddev.width - 24) && tp_dev.x[0] > (lcddev.width - 100) && tp_dev.y[0] > 250 && tp_dev.y[0] < 300)
		{
			LED1 = !LED1;
			if (!LED1)
			{
				ai_load_picfile("0:/PICTURE/buttomOn.jpg", 245, 285, 40, 40, 1); // 显示图片
			}
			else
			{
				ai_load_picfile("0:/PICTURE/buttomOff.jpg", 245, 285, 40, 40, 1); // 显示图片
			}
		}
		else if (tp_dev.x[0] > 100 && tp_dev.x[0] < 150 && tp_dev.y[0] > 370 && tp_dev.y[0] < 420)
		{
			BEEP = !BEEP;
			if (BEEP)
			{
				ai_load_picfile("0:/PICTURE/buttomOn.jpg", 90, 380, 40, 40, 1); // 显示图片
			}
			else
			{
				ai_load_picfile("0:/PICTURE/buttomOff.jpg", 90, 380, 40, 40, 1); // 显示图片
			}
		}
		else if (tp_dev.x[0] > (lcddev.width - 200) && tp_dev.x[0] < (lcddev.width - 50) && tp_dev.y[0] > 300 && tp_dev.y[0] < 450)
		{
			page = 2;
		}
	}
	if (MQ2_Scan() && BEEP != 1)
	{
		ai_load_picfile("0:/PICTURE/buttomOn.jpg", 90, 380, 40, 40, 1); // 显示图片
		BEEP = 1;
		RELAY = 0;
		// printf("烟雾浓度过高BEEP打开2");
	}
	// 与mqtt服务器交换数据
	MqttUpdate();
}

// 音乐页面
void PageMusic()
{

	u8 res;
	DIR mp3dir;			 // 目录
	FILINFO mp3fileinfo; // 文件信息
	u8 *fn;				 // 长文件名
	u8 *pname;			 // 带路径的文件名
	u16 totmp3num;		 // 音乐文件总数
	u16 curindex;		 // 图片当前索引
	u8 key;				 // 键值
	u16 temp;
	u16 *mp3indextbl;					   // 音乐索引表
	while (f_opendir(&mp3dir, "0:/MUSIC")) // 打开图片文件夹
	{
		Show_Str(30, 190, 240, 16, "MUSIC文件夹错误!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 190, 240, 206, WHITE); // 清除显示
		delay_ms(200);
	}
	totmp3num = mp3_get_tnum("0:/MUSIC"); // 得到总有效文件数
	while (totmp3num == NULL)			  // 音乐文件总数为0
	{
		Show_Str(30, 190, 240, 16, "没有音乐文件!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 190, 240, 146, WHITE); // 清除显示
		delay_ms(200);
	}
	mp3fileinfo.lfsize = _MAX_LFN * 2 + 1;									   // 长文件名最大长度
	mp3fileinfo.lfname = mymalloc(SRAMIN, mp3fileinfo.lfsize);				   // 为长文件缓存区分配内存
	pname = mymalloc(SRAMIN, mp3fileinfo.lfsize);							   // 为带路径的文件名分配内存
	mp3indextbl = mymalloc(SRAMIN, 2 * totmp3num);							   // 申请2*totmp3num个字节的内存,用于存放音乐文件索引
	while (mp3fileinfo.lfname == NULL || pname == NULL || mp3indextbl == NULL) // 内存分配出错
	{
		Show_Str(30, 190, 240, 16, "内存分配失败!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 190, 240, 146, WHITE); // 清除显示
		delay_ms(200);
	}
	VS_HD_Reset();
	VS_Soft_Reset();
	vsset.mvol = 150;					  // 默认设置音量为150.
	mp3_vol_show((vsset.mvol - 100) / 5); // 音量限制在:100~250,显示的时候,按照公式(vol-100)/5,显示,也就是0~30
	// 记录索引
	res = f_opendir(&mp3dir, "0:/MUSIC"); // 打开目录
	if (res == FR_OK)
	{
		curindex = 0; // 当前索引为0
		while (1)	  // 全部查询一遍
		{
			temp = mp3dir.index;					// 记录当前index
			res = f_readdir(&mp3dir, &mp3fileinfo); // 读取目录下的一个文件
			if (res != FR_OK || mp3fileinfo.fname[0] == 0)
				break; // 错误了/到末尾了,退出
			fn = (u8 *)(*mp3fileinfo.lfname ? mp3fileinfo.lfname : mp3fileinfo.fname);
			res = f_typetell(fn);
			if ((res & 0XF0) == 0X40) // 取高四位,看看是不是音乐文件
			{
				mp3indextbl[curindex] = temp; // 记录索引
				curindex++;
			}
		}
	}
	curindex = 0;										 // 从0开始显示
	res = f_opendir(&mp3dir, (const TCHAR *)"0:/MUSIC"); // 打开目录
	while (res == FR_OK)								 // 打开成功
	{
		dir_sdi(&mp3dir, mp3indextbl[curindex]); // 改变当前目录索引
		res = f_readdir(&mp3dir, &mp3fileinfo);	 // 读取目录下的一个文件
		if (res != FR_OK || mp3fileinfo.fname[0] == 0)
			return; // 错误了/到末尾了,退出
		fn = (u8 *)(*mp3fileinfo.lfname ? mp3fileinfo.lfname : mp3fileinfo.fname);
		strcpy((char *)pname, "0:/MUSIC/");		 // 复制路径(目录)
		strcat((char *)pname, (const char *)fn); // 将文件名接在后面
		LCD_Fill(60, 365, 440, 365 + 16, WHITE); // 清除之前的显示
		Show_Str(60, 365, 400, 16, fn, 16, 0);	 // 显示歌曲名字
		mp3_index_show(curindex + 1, totmp3num);
		key = mp3_play_song(pname); // 播放这个MP3
		if (key == 2)				// 上一曲
		{
			if (curindex)
				curindex--;
			else
				curindex = totmp3num - 1;
		}
		else if (key == 1) // 下一曲
		{
			curindex++;
			if (curindex >= totmp3num)
				curindex = 0; // 到末尾的时候,自动从头开始
		}
		else if (key == 0)
		{
			page = 1;
			break;
		}
		else
			return; // 产生了错误
	}
	myfree(SRAMIN, mp3fileinfo.lfname);
	myfree(SRAMIN, pname);
	myfree(SRAMIN, mp3indextbl);
}
// 页面选择
void PageSelect()
{
	switch (page)
	{
	case 1:

		if (page != previousPage)
		{
			Background(page); // 加载页面背景
			PagehomeInit();	  // 首页初始化
			previousPage = page;
		}
		Pagehome(); // 首页
		break;
	case 2:
		if (page != previousPage)
		{
			Background(page); // 加载页面背景
			printf("send:AT+CIPCLOSE\r\n");
			esp8266_send_cmd("AT+CIPCLOSE\r\n", "OK", 50); // 关闭连接
			previousPage = page;
		}
		PageMusic(); // 音乐页面
	default:
		break;
	}
}
// 背景选择
void Background(u8 page)
{
	switch (page)
	{
	case 1:
		// piclib_init();		//初始化画图
		res = f_opendir(&picdir, (const TCHAR *)"0:/PICTURE"); // 打开目录
		LCD_Clear(WHITE);
		ai_load_picfile("0:/PICTURE/Oneimg.jpg", 0, 0, lcddev.width, lcddev.height, 1); // 显示图片
		ai_load_picfile("0:/PICTURE/buttomOff.jpg", 245, 285, 40, 40, 1);				// 显示图片
		ai_load_picfile("0:/PICTURE/buttomOff.jpg", 90, 380, 40, 40, 1);				// 显示图片
		ai_load_picfile("0:/PICTURE/NextPage.jpg", 200, 350, 80, 80, 1);				// 显示图片
		//		myfree(SRAMIN,picfileinfo.lfname);	//释放内存
		//		myfree(SRAMIN,pname);				//释放内存
		//		myfree(SRAMIN,picindextbl);			//释放内存
		break;
	case 2:
		LCD_Clear(WHITE);
		ai_load_picfile("0:/PICTURE/MusicPage.jpg", 0, 0, lcddev.width, lcddev.height, 1); // 显示图片
		ai_load_picfile("0:/PICTURE/PreviousPage.jpg", 20, 20, 40, 40, 1);				   // 显示图片
	default:
		break;
	}
}
// 背景绘制初始化
void BackgroundInit()
{

	while (f_opendir(&picdir, "0:/PICTURE")) // 打开图片文件夹
	{
		Show_Str(30, 170, 240, 16, "PICTURE文件夹错误!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // 清除显示
		delay_ms(200);
	}
	totpicnum = pic_get_tnum("0:/PICTURE"); // 得到总有效文件数
	while (totpicnum == NULL)				// 图片文件为0
	{
		Show_Str(30, 170, 240, 16, "没有图片文件!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // 清除显示
		delay_ms(200);
	}
	picfileinfo.lfsize = _MAX_LFN * 2 + 1;									   // 长文件名最大长度
	picfileinfo.lfname = mymalloc(SRAMIN, picfileinfo.lfsize);				   // 为长文件缓存区分配内存
	pname = mymalloc(SRAMIN, picfileinfo.lfsize);							   // 为带路径的文件名分配内存
	picindextbl = mymalloc(SRAMIN, 2 * totpicnum);							   // 申请2*totpicnum个字节的内存,用于存放图片索引
	while (picfileinfo.lfname == NULL || pname == NULL || picindextbl == NULL) // 内存分配出错
	{
		Show_Str(30, 170, 240, 16, "内存分配失败!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // 清除显示
		delay_ms(200);
	}

	while (f_opendir(&picdir, "0:/PICTURE")) // 打开图片文件夹
	{
		Show_Str(30, 170, 240, 16, "PICTURE文件夹错误!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // 清除显示
		delay_ms(200);
	}
	totpicnum = pic_get_tnum("0:/PICTURE");									   // 得到总有效文件数
	picfileinfo.lfsize = _MAX_LFN * 2 + 1;									   // 长文件名最大长度
	picfileinfo.lfname = mymalloc(SRAMIN, picfileinfo.lfsize);				   // 为长文件缓存区分配内存
	pname = mymalloc(SRAMIN, picfileinfo.lfsize);							   // 为带路径的文件名分配内存
	picindextbl = mymalloc(SRAMIN, 2 * totpicnum);							   // 申请2*totpicnum个字节的内存,用于存放图片索引
	while (picfileinfo.lfname == NULL || pname == NULL || picindextbl == NULL) // 内存分配出错
	{
		Show_Str(30, 170, 240, 16, "内存分配失败!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // 清除显示
		delay_ms(200);
	}
	// 记录索引
	res = f_opendir(&picdir, "0:/PICTURE"); // 打开目录
	if (res == FR_OK)
	{
		curindex = 0; // 当前索引为0
		while (1)	  // 全部查询一遍
		{
			temp1 = picdir.index;					// 记录当前index
			res = f_readdir(&picdir, &picfileinfo); // 读取目录下的一个文件
			if (res != FR_OK || picfileinfo.fname[0] == 0)
				break; // 错误了/到末尾了,退出
			fn = (u8 *)(*picfileinfo.lfname ? picfileinfo.lfname : picfileinfo.fname);
			res = f_typetell(fn);
			if ((res & 0XF0) == 0X50) // 取高四位,看看是不是图片文件
			{
				picindextbl[curindex] = temp1; // 记录索引
				curindex++;
			}
		}
	}
}
