#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
/*
const char * id = "wifiid";
const char * password = "akjsdhasld";
*/
const char * id = "LAPTOP-GUUVA6FE 6259";
const char * password = "87654321";

void setup()
{
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.softAP(id,password);//����wifi
    //WiFi.begin(id,password);//��������
    /*
    while(WiFi.status()!=WL_CONNECTED)
    {
        delay(1000);
        Serial.print("�ȴ����ӡ�����");
    }
    */

    Serial.print("ID:");
    Serial.print(id);
    Serial.print("IP:");
    Serial.print(WiFi.softAPIP());//��������IP
    //Serial.print(WiFi.localIP());//��������IP

}
void loop()
{

}
