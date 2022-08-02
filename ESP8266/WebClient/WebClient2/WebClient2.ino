#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


void setup()
{
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);//设为无线终端模式
    WiFi.begin("laptop111","87654321");

    while(WiFi.status()!=WL_CONNECTED)
    {
        delay(1000);
        Serial.println("尝试连接。。。");
    }
    Serial.println("连接完成!");

    httpClientStart();
}

void loop()
{

}

void httpClientStart()
{
    
    WiFiClient client;
    String httpRequests="GET / HTTP/1.1\r\n Host: www.bilibili.com\r\nConnection: close\r\n\r\n ";
    
    if(client.connect("www.bilibili.com",80))
    {
        Serial.println("访问成功!");
        client.print(httpRequests);

        while(client.connected()||client.available())
        {
            if(client.available())
            {
                String line=client.readStringUntil('\n');
                Serial.print(line);
            }
        }
        client.stop();
    }
    else
    {
        Serial.println("连接失败!");
    }
    
}