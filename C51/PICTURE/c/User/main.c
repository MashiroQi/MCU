/*
	作者：MashiroQi
	配合Python脚本使用
	还不是很完善
	之后会继续更新
*/
#include "public.h"
#include "LCD12864.h"
#include "serial.h"

static uchar row=0;
static uchar col=0;
void main()
{
	lcd_init();

	UartInit();
	lcd_wcmd(0x36);
	
	lcd_wcmd(0x80);
	lcd_wcmd(0x80);
	while(1);

}


void uart() interrupt 4 //串口通信中断函数
{

		
		uchar rec_data;
		rec_data=SBUF;	//存储接收到的数据
		RI = 0;			//清除接收中断标志位
		if(row==64)
		{
			row=0;
			lcd_wcmd(0x36);
		}
		if(col ==16)      				
		{
			col=0;
			row++;
			lcd_wcmd(0x80 + ((row / 32) ? (row - 32) : row));
			lcd_wcmd((row / 32) ? 0x88 : 0x80);		
		}
		col++;
		lcd_wdat(rec_data);

		/*
		SBUF=rec_data;	//将接收到的数据放入到发送寄存器
		while(!TI);		//等待发送数据完成
		TI=0;			//清除发送完成标志位
		*/
}

