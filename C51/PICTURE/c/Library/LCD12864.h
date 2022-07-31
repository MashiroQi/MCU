#ifndef LCD12864_H
#define LCD12864_H

void lcd_showNum(uchar line,uchar column,uint num,uchar len);
void lcd_showSignedNum(uchar line,uchar column,int num,uchar len);
void lcd_showString(uchar line,uchar column,uchar *string);
void lcd_showChar(uchar line,uchar column,uchar c);
void lcd_clear();
void lcd_init();

#define lcd_dat P0

sbit lcd_rs=P2^3;
sbit lcd_rw=P2^4;
sbit lcd_en=P2^5;
/*
bit lcd_busy()
{
	bit r;
	lcd_dat=0x00;
	lcd_rs=0;
	lcd_rw=1;
	lcd_en=1;
	r=(bit)(lcd_dat&0x80);
	lcd_en=0;
	return r;
}
*/
void lcd_wcmd(uchar cmd)
{
	//while(lcd_busy());
	lcd_rs=0;
	lcd_rw=0;
	lcd_en=0;
	//_nop_();
	//_nop_();
	lcd_dat=cmd;
	lcd_en=1;
	lcd_en=0;
}
void lcd_wdat(uchar dat)
{
	//while(lcd_busy());
	lcd_rs=1;
	lcd_rw=0;
	lcd_en=0;
	lcd_dat=dat;
	lcd_en=1;
	lcd_en=0;
}

void lcd_init()
{
	lcd_wcmd(0x34);
	delay_ms(5);
	lcd_wcmd(0x30);
	delay_ms(5);
	lcd_wcmd(0x0c);
	delay_ms(5);
	lcd_wcmd(0x01);
	delay_ms(5);
}

void lcd_pos(uchar x,uchar y)
{
	char pos;
	if(x==0)x=0x80;
	else if(x==1)x=0x90;
	else if(x==2)x=0x88;
	else if(x==3)x=0x98;
	pos=x+y;
	lcd_wcmd(pos);
}

int lcd_pow(uchar x,uchar y)
{
	uchar i;
	int r=1;
	for(i=0;i<y;i++)r*=x;
	return r;
}
void lcd_showNum(uchar line,uchar column,uint num,uchar len)
{
	uchar i;
	lcd_pos(line,column);
	for(i=len;i>0;i--)
	{
		lcd_wdat(num/lcd_pow(10,i-1)%10+'0');
	}
}

void lcd_showSignedNum(uchar line,uchar column,int num,uchar len)
{
	uchar i;
	int num1;
	lcd_pos(line,column);
	if(num>0)
	{
		lcd_wdat('+');
		num1=num;
	}
	if(num<0)
	{
		lcd_wdat('-');
		num1=-num;
	}
	for(i=len;i>0;i--)
	{
		lcd_wdat(num1/lcd_pow(10,i-1)%10+'0');
	}
	
}
void lcd_showString(uchar line,uchar column,uchar *string)
{
	uchar i;
	lcd_pos(line,column);
	for(i=0;string[i]!='\0';i++)
	{
		lcd_wdat(string[i]);
	}
}
void lcd_showChar(uchar line,uchar column,uchar c)
{
	lcd_pos(line,column);
	lcd_wdat(c);
}

void lcd_clear()
{
	lcd_wdat(0x01);
}

void lcd_showImage(uchar *image)
{
	uint x=0;
	uchar row=0;
	uchar col=0;
	lcd_wcmd(0x36);
	for(row=0;row<64;row++)
	{
		lcd_wcmd(0x80 + ((row / 32) ? (row - 32) : row));
		lcd_wcmd((row / 32) ? 0x88 : 0x80);
		for(col = 0; col < 16; col++)      				
		{           
			lcd_wdat(image[x++]);
		} 
	}
}
#endif