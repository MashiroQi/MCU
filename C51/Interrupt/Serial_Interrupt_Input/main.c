#include <REGX51.H>
#include "stdio.h"

#define BUFFER_SIZE 64
char buffer[BUFFER_SIZE] = {0};
int buffer_arr;
char ReceiveFlag;
// �����жϳ�ʼ��
void Serial_init()
{
    TMOD |= 0X20;
    TH1 = 0Xfd;
    TL1 = 0Xfd; // ���ò�����9600
    TR1 = 1;    // ����ʱ��1�ж�
    SM0 = 0;    // ������ʽ1
    SM1 = 1;
    ES = 1; // ��ʼ�����ж�
    EA = 1; // �����ж�
    TI = 1;
}

int main() // ������
{
    Serial_init(); // ���ڳ�ʼ��
    ReceiveFlag = 0;
    while (1)
    {
        if (ReceiveFlag)
        {
            printf("%s", buffer);
        }
    }
}
void SerialInterrpt() interrupt 4
{
    char rec;
    rec = SBUF;
    buffer[buffer_arr] = rec;
    buffer_arr++;
    RI = 0;

    if (buffer_arr == BUFFER_SIZE)
    {
        buffer_arr = 0; // ���½���;
    }
    if (rec == '\n' || rec == '\0')
    {
        buffer[buffer_arr] = '\0';
        buffer_arr = 0; // ���½���
        ReceiveFlag = 1;
    }
}
