#ifndef __MQ2_H
#define __MQ2_H
#include "sys.h"

#define MQ2  PGin(9)//��ȡMQ2

void MQ2_Init(void);//IO��ʼ��
u8 MQ2_Scan(void);  	//ɨ�躯��					    
#endif
