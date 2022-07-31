/*
	���ߣ�MashiroQi
	���Python�ű�ʹ��
	�����Ǻ�����
	֮����������
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


void uart() interrupt 4 //����ͨ���жϺ���
{

		
		uchar rec_data;
		rec_data=SBUF;	//�洢���յ�������
		RI = 0;			//��������жϱ�־λ
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
		SBUF=rec_data;	//�����յ������ݷ��뵽���ͼĴ���
		while(!TI);		//�ȴ������������
		TI=0;			//���������ɱ�־λ
		*/
}

