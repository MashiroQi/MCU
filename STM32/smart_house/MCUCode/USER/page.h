#ifndef __PAGE_H
#define __PAGE_H
#include "ff.h"

extern int pwm_servo;
extern u8 page;
void Show_Weather(void);
void ShowTempHumi(void);
void Background(u8 page);
void MqttInit(void);
void MqttUpdate(void);
void PageSelect();
void BackgroundInit(void);
#endif
