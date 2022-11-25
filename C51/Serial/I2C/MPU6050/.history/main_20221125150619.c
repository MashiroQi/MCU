#include <REG52.H>

sbit SDA = ;
sbit SCL = ;

void Serial_init();
void main()
{
    Serial_init();
    while (1)
    {
    }
}
void delay(unsigned int x)
{
    while (x--)
        ;
}
void IIC_Start()
{
    SDA = 1;
    SCL = 1;
    delay(1);
    SDA = 0;
    delay(1);
    SCL = 0;
}
void IIC_Stop()
{
    SDA = 0;
    SCL = 1;
    delay(1);
    SDA = 1;
}

unsigned char IIC_Wait_Ack()
{
    unsigned char re;
    SDA = 1;
    delay(1);
    SCL = 1;
    delay(1);
    if (SDA)
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    SCL = 0;
    delay(1);
    return re;
}
void Ack()
{
    SDA = 0;
    delay(1);
    SCL = 1;
    delay(1);
    SCL = 0;
    delay(1);
    SDA = 1;
}
void NoAck()
{
    SDA = 1;
    delay(1);
    SCL = 1;
    delay(1);
    SCL = 0;
    delay(1);
}
void IIC_Send_Byte(unsigned char dat)
{
    unsigned char i = 0;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
        {
            SDA = 1;
        }
        else
        {
            SDA = 0;
        }
        delay(1);
        SCL = 1;
        delay(1);
        SCL = 0;
        if (i == 7)
        {
            SDA = 1;
        }
        dat <<= 1;
        delay(1);
    }
}

unsigned char IIC_Read_Byte(unsigned char ack)
{
    unsigned char i = 0;
    unsigned char dat;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        SCL = 1;
        delay(1);
        if (SDA == 1)
        {
            dat += 1;
        }
    }
    if (ack)
    {
        ACK();
    }
    else
    {
        NoAck();
    }
    return dat;
}
void IIC_WriteByte(unsigned char addr, unsigned char dat)
{
    IIC_Start();
    IIC_Send_Byte(addr);
    IIC_Wait_Ack();
    IIC_Send_Byte(addr);
    IIC_Wait_Ack();
    IIC_Send_Byte(dat);
    IIC_Wait_Ack();
    IIC_Stop();
}

void MPU6050_ReadData(unsigned char reg_add, unsigned char *Read, unsigned char num)
{
    unsigned char i;

    IIC_Start();
    IIC_Send_Byte(0x68);
    IIC_Wait_Ack();
    IIC_Send_Byte(reg_add);
    IIC_Wait_Ack();

    IIC_Start();
    IIC_Send_Byte(0x68 + 1);
    IIC_Wait_Ack();

    for (i = 0; i < (num - 1); i++)
    {
        *Read = IIC_Read_Byte(1);
        Read++;
    }
    *Read = IIC_Read_Byte(0);
    IIC_Stop();
}
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