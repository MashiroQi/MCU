#include <Arduino.h>
#line 1 "d:\\gitproject\\MCU\\Arduino\\bai\\bai.ino"
/*µ¹Á¢°Ú*/
#include "pid.h"

#define AngleSensor A0
#define Motor A1
int measure;
int target = 180;
pidStruct *pid;
#line 9 "d:\\gitproject\\MCU\\Arduino\\bai\\bai.ino"
void setup();
#line 17 "d:\\gitproject\\MCU\\Arduino\\bai\\bai.ino"
void loop();
#line 9 "d:\\gitproject\\MCU\\Arduino\\bai\\bai.ino"
void setup()
{
  pinMode(AngleSensor, INPUT);
  pinMode(Motor, OUTPUT);
  Serial.begin(115200);
  pid = pidInit();
}

void loop()
{
  measure = analogRead(AngleSensor);
  if (!((target - measure) < 5.0) && ((target - measure) > -5.0))
  {
    analogWrite(Motor, (int)pid_realize(target, float(measure), pid));
    Serial.println(measure);
  }
}

