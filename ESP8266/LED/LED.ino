#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

void setup()
{
    pinMode(LED_BUILTIN,OUTPUT);
    Serial.begin(9600);
    //WiFi.mode(WIFI_STA);
    wifiMulti.addAP("laptop111","87654321");
    wifiMulti.addAP("mashirotest","mashiroqi");
    while (wifiMulti.run() != WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
        delay(1000);
        digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
        Serial.print("Connecting...");
    }
    digitalWrite(LED_BUILTIN,LOW);
    Serial.print("Connect established!");
    Serial.print(WiFi.localIP());

    server.begin();
    server.on("/",HTTP_GET,handleRoot);
    server.on("/LED",HTTP_POST,handleLED);
    server.onNotFound(handleNotFound);
}

void loop()
{
    server.handleClient();
}

void handleRoot()
{
        server.send(200,"text/html","<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"LED\"></form>");
}

void handleNotFound()
{
    server.send(404,"text/plain","404:Not Found");
}

void handleLED()
{
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  server.sendHeader("Location","/");
  server.send(303);
}