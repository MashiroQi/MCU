# 1 "d:\\gitproject\\MCU\\Arduino\\test\\bai.ino"
/*µ¹Á¢°Ú*/
# 3 "d:\\gitproject\\MCU\\Arduino\\test\\bai.ino" 2


void setup()
{
  pinMode(3, 0x1);
  pinMode(4, 0x1);
  pinMode(5, 0x1);
  Serial.begin(115200);
}

void loop()
{
  analogWrite(3, 255);
  digitalWrite(4, 0x1);
  digitalWrite(5, 0x0);
}
