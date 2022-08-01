#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);


void setup()
{
    Serial.begin(9600);
    //WiFi.mode(WIFI_STA);
    wifiMulti.addAP("laptop111","87654321");
    wifiMulti.addAP("mashirotest","mashiroqi");
    while (wifiMulti.run() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print("连接中。。。");
    }

    Serial.print(WiFi.localIP());

    server.begin();
    server.on("/",handleRoot);
    server.onNotFound(handleNotFound);
}

void loop()
{
    server.handleClient();
}

void handleRoot()
{
    server.send(200,"text/plain","Hello World!");
}

void handleNotFound()
{
    server.send(404,"text/plain","404:Not Found");
}