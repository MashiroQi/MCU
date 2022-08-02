#include <FS.h>

String file_name="/folder/test.txt";
String folder_name="/folder";


void setup()
{
    Serial.begin(9600);
    SPIFFS.format();
    Serial.println("SPIFFS format finish!");

    if(SPIFFS.begin())
    {
        Serial.println("Started");
    }
    else
    {
        Serial.println("Failed");
    }

    Dir makeDir = SPIFFS.openDir(folder_name);

    File writeFile = SPIFFS.open(file_name,"w");
    writeFile.println("Hello World!");
    writeFile.close();
    Serial.println("写入完成!");

    if(SPIFFS.exists(file_name))
    {
        Serial.println(file_name+"found");
    }
    else
    {
        Serial.println(file_name+"not found");
    }

    File readFile = SPIFFS.open(file_name,"r");
    
    for(int i=0;i<readFile.size();i++)
    {
        Serial.print((char)readFile.read());
    } 
    readFile.close();
    Serial.println("读取完成!");

    Serial.println("读取文件夹");
    while(makeDir.next())
    {
        Serial.println(makeDir.fileName());
    }
    Serial.println("读取完成");
}

void loop()
{
    
}
