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

u16 totpicnum; // ͼƬ�ļ�����
u8 res;
DIR picdir;			 // ͼƬĿ¼
FILINFO picfileinfo; // �ļ���Ϣ
u8 *fn;				 // ���ļ���
u8 *pname;			 // ��·�����ļ���
u16 curindex;		 // ͼƬ��ǰ����
u8 temp1;
u16 *picindextbl; // ͼƬ������

u8 PUB_BUF[250];
u8 temp, humi;
const char *topics[] = {"/home/sub"};
unsigned short timeCount = 0; // ���ͼ������
u8 *dataPtr = NULL;
u8 page = 1;
u8 previousPage = 0;
u8 weatherText[10];

int pwm_servo = 570; // min570 max2800

// �������
void TouchScan()
{
	tp_dev.scan(0); // ɨ�败����
	if (tp_dev.sta & TP_PRES_DOWN)
	{
		if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height)
		{
			//			printf("x=%d,y=%d",tp_dev.x,tp_dev.y);
			//			if(tp_dev.x[0]>0)
			//			TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//��ͼ
		}
	}
}

// mqtt�ӿڳ�ʼ��
void MqttInit()
{
	ESP8266_Init();			 // esp8266
	while (OneNet_DevLink()) // ����mqtt������
		delay_ms(1000);
	OneNet_Subscribe(topics, 1); // ��������
}

// ��mqtt��������������
void MqttUpdate()
{
	if (++timeCount >= 200) // ���ͼ��5s
	{
		// UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
		ShowTempHumi();
		sprintf(PUB_BUF, "{\"Temp\":%d ,\"Humi\":%d,\"Smoke\":%d,\"Servo\":%d}", temp, humi, MQ2_Scan(), pwm_servo > 1000 ? 0 : 1);
		// ������ʪ��
		OneNet_Publish("/home/pub", PUB_BUF);
		timeCount = 0;
		ESP8266_Clear();
	}
	// �ȴ���������
	dataPtr = ESP8266_GetIPD(10);
	if (dataPtr != NULL)
	{
		printf("%s", dataPtr);
		OneNet_RevPro(dataPtr); // �����������
	}
}

// ��ʾ����
void Show_Weather()
{
	cJSON *root = NULL;
	cJSON *root1 = NULL;
	cJSON *root2 = NULL;
	if (strlen(weatherText) < 2)
	{
		get_current_weather();
		//		printf("JSON_BUF=%s",JSON_BUF);
		// ��������JSON����
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
	// ��ȡJSON���¶�
	//	root2 = cJSON_GetObjectItem(root1, "temperature");
	//	if(root2->child != NULL)
	//		printf("temperature=%s\n", cJSON_Print(root2));
	//	Show_Str(90,290,200,16,root2->valuestring,16,1);
	cJSON_Delete(root);
}

// ��ʾ�¶�ʪ��
void ShowTempHumi()
{
	do
	{
		DHT11_Read_Data(&temp, &humi);
		LCD_ShowNum(90, 190, temp, 2, 24);
		LCD_ShowNum(250, 190, humi, 2, 24);
	} while (humi == 0);
}

// ��ҳ��ʼ��
void PagehomeInit()
{

	POINT_COLOR = WHITE;
	Show_Str_Mid(45, 70, "���ܼҾ�", 24, 500);
	POINT_COLOR = BLACK;
	Show_Str(90, 190, 200, 24, "  ��", 24, 1);
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

// ��ҳ
void Pagehome()
{
	TouchScan(); // ɨ�败����
	if (tp_dev.sta & TP_PRES_DOWN)
	{
		if (tp_dev.x[0] < (lcddev.width - 24) && tp_dev.x[0] > (lcddev.width - 100) && tp_dev.y[0] > 250 && tp_dev.y[0] < 300)
		{
			LED1 = !LED1;
			if (!LED1)
			{
				ai_load_picfile("0:/PICTURE/buttomOn.jpg", 245, 285, 40, 40, 1); // ��ʾͼƬ
			}
			else
			{
				ai_load_picfile("0:/PICTURE/buttomOff.jpg", 245, 285, 40, 40, 1); // ��ʾͼƬ
			}
		}
		else if (tp_dev.x[0] > 100 && tp_dev.x[0] < 150 && tp_dev.y[0] > 370 && tp_dev.y[0] < 420)
		{
			BEEP = !BEEP;
			if (BEEP)
			{
				ai_load_picfile("0:/PICTURE/buttomOn.jpg", 90, 380, 40, 40, 1); // ��ʾͼƬ
			}
			else
			{
				ai_load_picfile("0:/PICTURE/buttomOff.jpg", 90, 380, 40, 40, 1); // ��ʾͼƬ
			}
		}
		else if (tp_dev.x[0] > (lcddev.width - 200) && tp_dev.x[0] < (lcddev.width - 50) && tp_dev.y[0] > 300 && tp_dev.y[0] < 450)
		{
			page = 2;
		}
	}
	if (MQ2_Scan() && BEEP != 1)
	{
		ai_load_picfile("0:/PICTURE/buttomOn.jpg", 90, 380, 40, 40, 1); // ��ʾͼƬ
		BEEP = 1;
		RELAY = 0;
		// printf("����Ũ�ȹ���BEEP��2");
	}
	// ��mqtt��������������
	MqttUpdate();
}

// ����ҳ��
void PageMusic()
{

	u8 res;
	DIR mp3dir;			 // Ŀ¼
	FILINFO mp3fileinfo; // �ļ���Ϣ
	u8 *fn;				 // ���ļ���
	u8 *pname;			 // ��·�����ļ���
	u16 totmp3num;		 // �����ļ�����
	u16 curindex;		 // ͼƬ��ǰ����
	u8 key;				 // ��ֵ
	u16 temp;
	u16 *mp3indextbl;					   // ����������
	while (f_opendir(&mp3dir, "0:/MUSIC")) // ��ͼƬ�ļ���
	{
		Show_Str(30, 190, 240, 16, "MUSIC�ļ��д���!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 190, 240, 206, WHITE); // �����ʾ
		delay_ms(200);
	}
	totmp3num = mp3_get_tnum("0:/MUSIC"); // �õ�����Ч�ļ���
	while (totmp3num == NULL)			  // �����ļ�����Ϊ0
	{
		Show_Str(30, 190, 240, 16, "û�������ļ�!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 190, 240, 146, WHITE); // �����ʾ
		delay_ms(200);
	}
	mp3fileinfo.lfsize = _MAX_LFN * 2 + 1;									   // ���ļ�����󳤶�
	mp3fileinfo.lfname = mymalloc(SRAMIN, mp3fileinfo.lfsize);				   // Ϊ���ļ������������ڴ�
	pname = mymalloc(SRAMIN, mp3fileinfo.lfsize);							   // Ϊ��·�����ļ��������ڴ�
	mp3indextbl = mymalloc(SRAMIN, 2 * totmp3num);							   // ����2*totmp3num���ֽڵ��ڴ�,���ڴ�������ļ�����
	while (mp3fileinfo.lfname == NULL || pname == NULL || mp3indextbl == NULL) // �ڴ�������
	{
		Show_Str(30, 190, 240, 16, "�ڴ����ʧ��!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 190, 240, 146, WHITE); // �����ʾ
		delay_ms(200);
	}
	VS_HD_Reset();
	VS_Soft_Reset();
	vsset.mvol = 150;					  // Ĭ����������Ϊ150.
	mp3_vol_show((vsset.mvol - 100) / 5); // ����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30
	// ��¼����
	res = f_opendir(&mp3dir, "0:/MUSIC"); // ��Ŀ¼
	if (res == FR_OK)
	{
		curindex = 0; // ��ǰ����Ϊ0
		while (1)	  // ȫ����ѯһ��
		{
			temp = mp3dir.index;					// ��¼��ǰindex
			res = f_readdir(&mp3dir, &mp3fileinfo); // ��ȡĿ¼�µ�һ���ļ�
			if (res != FR_OK || mp3fileinfo.fname[0] == 0)
				break; // ������/��ĩβ��,�˳�
			fn = (u8 *)(*mp3fileinfo.lfname ? mp3fileinfo.lfname : mp3fileinfo.fname);
			res = f_typetell(fn);
			if ((res & 0XF0) == 0X40) // ȡ����λ,�����ǲ��������ļ�
			{
				mp3indextbl[curindex] = temp; // ��¼����
				curindex++;
			}
		}
	}
	curindex = 0;										 // ��0��ʼ��ʾ
	res = f_opendir(&mp3dir, (const TCHAR *)"0:/MUSIC"); // ��Ŀ¼
	while (res == FR_OK)								 // �򿪳ɹ�
	{
		dir_sdi(&mp3dir, mp3indextbl[curindex]); // �ı䵱ǰĿ¼����
		res = f_readdir(&mp3dir, &mp3fileinfo);	 // ��ȡĿ¼�µ�һ���ļ�
		if (res != FR_OK || mp3fileinfo.fname[0] == 0)
			return; // ������/��ĩβ��,�˳�
		fn = (u8 *)(*mp3fileinfo.lfname ? mp3fileinfo.lfname : mp3fileinfo.fname);
		strcpy((char *)pname, "0:/MUSIC/");		 // ����·��(Ŀ¼)
		strcat((char *)pname, (const char *)fn); // ���ļ������ں���
		LCD_Fill(60, 365, 440, 365 + 16, WHITE); // ���֮ǰ����ʾ
		Show_Str(60, 365, 400, 16, fn, 16, 0);	 // ��ʾ��������
		mp3_index_show(curindex + 1, totmp3num);
		key = mp3_play_song(pname); // �������MP3
		if (key == 2)				// ��һ��
		{
			if (curindex)
				curindex--;
			else
				curindex = totmp3num - 1;
		}
		else if (key == 1) // ��һ��
		{
			curindex++;
			if (curindex >= totmp3num)
				curindex = 0; // ��ĩβ��ʱ��,�Զ���ͷ��ʼ
		}
		else if (key == 0)
		{
			page = 1;
			break;
		}
		else
			return; // �����˴���
	}
	myfree(SRAMIN, mp3fileinfo.lfname);
	myfree(SRAMIN, pname);
	myfree(SRAMIN, mp3indextbl);
}
// ҳ��ѡ��
void PageSelect()
{
	switch (page)
	{
	case 1:

		if (page != previousPage)
		{
			Background(page); // ����ҳ�汳��
			PagehomeInit();	  // ��ҳ��ʼ��
			previousPage = page;
		}
		Pagehome(); // ��ҳ
		break;
	case 2:
		if (page != previousPage)
		{
			Background(page); // ����ҳ�汳��
			printf("send:AT+CIPCLOSE\r\n");
			esp8266_send_cmd("AT+CIPCLOSE\r\n", "OK", 50); // �ر�����
			previousPage = page;
		}
		PageMusic(); // ����ҳ��
	default:
		break;
	}
}
// ����ѡ��
void Background(u8 page)
{
	switch (page)
	{
	case 1:
		// piclib_init();		//��ʼ����ͼ
		res = f_opendir(&picdir, (const TCHAR *)"0:/PICTURE"); // ��Ŀ¼
		LCD_Clear(WHITE);
		ai_load_picfile("0:/PICTURE/Oneimg.jpg", 0, 0, lcddev.width, lcddev.height, 1); // ��ʾͼƬ
		ai_load_picfile("0:/PICTURE/buttomOff.jpg", 245, 285, 40, 40, 1);				// ��ʾͼƬ
		ai_load_picfile("0:/PICTURE/buttomOff.jpg", 90, 380, 40, 40, 1);				// ��ʾͼƬ
		ai_load_picfile("0:/PICTURE/NextPage.jpg", 200, 350, 80, 80, 1);				// ��ʾͼƬ
		//		myfree(SRAMIN,picfileinfo.lfname);	//�ͷ��ڴ�
		//		myfree(SRAMIN,pname);				//�ͷ��ڴ�
		//		myfree(SRAMIN,picindextbl);			//�ͷ��ڴ�
		break;
	case 2:
		LCD_Clear(WHITE);
		ai_load_picfile("0:/PICTURE/MusicPage.jpg", 0, 0, lcddev.width, lcddev.height, 1); // ��ʾͼƬ
		ai_load_picfile("0:/PICTURE/PreviousPage.jpg", 20, 20, 40, 40, 1);				   // ��ʾͼƬ
	default:
		break;
	}
}
// �������Ƴ�ʼ��
void BackgroundInit()
{

	while (f_opendir(&picdir, "0:/PICTURE")) // ��ͼƬ�ļ���
	{
		Show_Str(30, 170, 240, 16, "PICTURE�ļ��д���!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // �����ʾ
		delay_ms(200);
	}
	totpicnum = pic_get_tnum("0:/PICTURE"); // �õ�����Ч�ļ���
	while (totpicnum == NULL)				// ͼƬ�ļ�Ϊ0
	{
		Show_Str(30, 170, 240, 16, "û��ͼƬ�ļ�!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // �����ʾ
		delay_ms(200);
	}
	picfileinfo.lfsize = _MAX_LFN * 2 + 1;									   // ���ļ�����󳤶�
	picfileinfo.lfname = mymalloc(SRAMIN, picfileinfo.lfsize);				   // Ϊ���ļ������������ڴ�
	pname = mymalloc(SRAMIN, picfileinfo.lfsize);							   // Ϊ��·�����ļ��������ڴ�
	picindextbl = mymalloc(SRAMIN, 2 * totpicnum);							   // ����2*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ����
	while (picfileinfo.lfname == NULL || pname == NULL || picindextbl == NULL) // �ڴ�������
	{
		Show_Str(30, 170, 240, 16, "�ڴ����ʧ��!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // �����ʾ
		delay_ms(200);
	}

	while (f_opendir(&picdir, "0:/PICTURE")) // ��ͼƬ�ļ���
	{
		Show_Str(30, 170, 240, 16, "PICTURE�ļ��д���!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // �����ʾ
		delay_ms(200);
	}
	totpicnum = pic_get_tnum("0:/PICTURE");									   // �õ�����Ч�ļ���
	picfileinfo.lfsize = _MAX_LFN * 2 + 1;									   // ���ļ�����󳤶�
	picfileinfo.lfname = mymalloc(SRAMIN, picfileinfo.lfsize);				   // Ϊ���ļ������������ڴ�
	pname = mymalloc(SRAMIN, picfileinfo.lfsize);							   // Ϊ��·�����ļ��������ڴ�
	picindextbl = mymalloc(SRAMIN, 2 * totpicnum);							   // ����2*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ����
	while (picfileinfo.lfname == NULL || pname == NULL || picindextbl == NULL) // �ڴ�������
	{
		Show_Str(30, 170, 240, 16, "�ڴ����ʧ��!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 170, 240, 186, WHITE); // �����ʾ
		delay_ms(200);
	}
	// ��¼����
	res = f_opendir(&picdir, "0:/PICTURE"); // ��Ŀ¼
	if (res == FR_OK)
	{
		curindex = 0; // ��ǰ����Ϊ0
		while (1)	  // ȫ����ѯһ��
		{
			temp1 = picdir.index;					// ��¼��ǰindex
			res = f_readdir(&picdir, &picfileinfo); // ��ȡĿ¼�µ�һ���ļ�
			if (res != FR_OK || picfileinfo.fname[0] == 0)
				break; // ������/��ĩβ��,�˳�
			fn = (u8 *)(*picfileinfo.lfname ? picfileinfo.lfname : picfileinfo.fname);
			res = f_typetell(fn);
			if ((res & 0XF0) == 0X50) // ȡ����λ,�����ǲ���ͼƬ�ļ�
			{
				picindextbl[curindex] = temp1; // ��¼����
				curindex++;
			}
		}
	}
}
