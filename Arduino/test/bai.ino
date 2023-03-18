/*µ¹Á¢°Ú*/
#include "pid.h"

#define Motor 3
void setup()
{
  pinMode(Motor, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  analogWrite(Motor, 255);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
}
