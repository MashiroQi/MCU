#ifndef SERIAL_H
#define SERIAL_H

void UartInit()
{
	TMOD|=0X20;	//设置计数器工作方式2
	SCON=0X50;	//设置为工作方式1
	PCON=0X80;	//波特率加倍
	TH1=0xff;	//计数器初始值设置
	TL1=0xff;
	ES=1;		//打开接收中断
	EA=1;		//打开总中断
	TR1=1;		//打开计数器		
}

#endif