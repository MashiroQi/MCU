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
    HTTPClient httpClient;
    httpClient.begin(client,"http://124.223.169.31/");
    int httpCode=httpClient.GET();

    if(httpCode==HTTP_CODE_OK)
    {
        Serial.println("访问成功!");
        String responsePayload=httpClient.getString();
        Serial.println(responsePayload);
    }
    else
    {
        Serial.println(httpCode);
    }
    httpClient.end();
}