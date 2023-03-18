# 1 "d:\\gitproject\\MCU\\Arduino\\bai\\bai.ino"
/*µ¹Á¢°Ú*/
# 3 "d:\\gitproject\\MCU\\Arduino\\bai\\bai.ino" 2



int measure;
int target = 180;
pidStruct *pid;
void setup()
{
  pinMode(A0, 0x0);
  pinMode(A1, 0x1);
  Serial.begin(115200);
  pid = pidInit();
}

void loop()
{
  measure = analogRead(A0);
  if (!((target - measure) < 5.0) && ((target - measure) > -5.0))
  {
    analogWrite(A1, (int)pid_realize(target, float(measure), pid));
    Serial.println(measure);
  }
}
