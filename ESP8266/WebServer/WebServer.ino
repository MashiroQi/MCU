#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80); 

void setup()
{
    Serial.begin(9600);
    wifiMulti.addAP("laptop111","87654321");
    
    while(wifiMulti.run()!=WL_CONNECTED)
    {
        delay(1000);
        Serial.println("连接失败!重新尝试连接");
    }
    Serial.println("连接完成!;IP:");
    Serial.println(WiFi.localIP());
    Serial.println("ID:"+WiFi.SSID());

    if(SPIFFS.begin())
    {
        Serial.println("SPIFFS Started!");
    }
    else
    {
        Serial.println("SPIFFS Failed to Start!");
    }
    
    server.onNotFound(handleRequests);
    server.begin();
    Serial.println("服务开始运行!");
}

void loop()
{
    server.handleClient();
}
void handleRequests()
{
    String reqResource=server.uri();
    Serial.println("用户请求:"+reqResource);
    
    bool fileReadOK=handleReadFile(reqResource);

    if(!fileReadOK)
    {
        server.send(404,"text/plain","404:Not Found");
    }

}

bool handleReadFile(String resource)
{
    if(resource.endsWith("/"))
    {
        resource="/index.html";
    }

    String contentType=getContentType(resource);

    if(SPIFFS.exists(resource))
    {
        File file=SPIFFS.open(resource,"r");
        server.streamFile(file,contentType);
        file.close();
        return true;
    }
    Serial.println(resource+"不存在");
    return false;
}

String getContentType(String resource)
{
    if(resource.endsWith(".htm"))return "text/html";
    if(resource.endsWith(".html"))return "text/html";
    if(resource.endsWith(".css"))return "text/css";
    if(resource.endsWith(".js"))return "application/javascript";
    if(resource.endsWith(".png"))return "image/png";
    if(resource.endsWith(".gif"))return "image/gif";
    if(resource.endsWith(".jpg"))return "image/jpeg";
    if(resource.endsWith(".ico"))return "image/x-icon";
    if(resource.endsWith(".xml"))return "text/xml";
    if(resource.endsWith(".pdf"))return "application/x-pdf";
    if(resource.endsWith(".zip"))return "application/x-zip";
    if(resource.endsWith(".gz"))return "application/x-gzip";
    return "text/plain";
}
