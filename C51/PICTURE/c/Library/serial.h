#ifndef SERIAL_H
#define SERIAL_H

void UartInit()
{
	TMOD|=0X20;	//���ü�����������ʽ2
	SCON=0X50;	//����Ϊ������ʽ1
	PCON=0X80;	//�����ʼӱ�
	TH1=0xff;	//��������ʼֵ����
	TL1=0xff;
	ES=1;		//�򿪽����ж�
	EA=1;		//�����ж�
	TR1=1;		//�򿪼�����		
}

#endif