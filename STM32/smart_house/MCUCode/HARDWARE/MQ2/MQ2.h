#ifndef __MQ2_H
#define __MQ2_H
#include "sys.h"

#define MQ2  PGin(9)//读取MQ2

void MQ2_Init(void);//IO初始化
u8 MQ2_Scan(void);  	//扫描函数					    
#endif
