/*µ¹Á¢°Ú*/
#include "pid.h"

#define AngleSensor 3
#define Motor A1
int measure;
int target = 90;
int out;
pidStruct *pid;
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
    out = (int)pid_realize(target, float(measure), pid);
    analogWrite(Motor, out);
    Serial.write("Measure:");
    Serial.println(measure);
    Serial.write("Out:");
    Serial.println(out);
  }
}
