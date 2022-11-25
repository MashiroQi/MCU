#include <REGX52.H>
#include <stdio.h>
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
sbit SCL = P1 ^ 5;        // IICʱ�����Ŷ���
sbit SDA = P1 ^ 4;        // IIC�������Ŷ���
#define SMPLRT_DIV 0x19   // �����ǲ����ʣ�����ֵ��0x07(125Hz)
#define CONFIG 0x1A       // ��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define GYRO_CONFIG 0x1B  // �������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define ACCEL_CONFIG 0x1C // ���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define PWR_MGMT_1 0x6B   // ��Դ��������ֵ��0x00(��������)
#define WHO_AM_I 0x75     // IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define SlaveAddress 0xD0 // IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

uchar dis[6]; // ��ʾ����(-511��512)���ַ�����
int dis_data; // ����

void Serial_init()
{
    TMOD |= 0X20;
    TH1 = 0Xfd;
    TL1 = 0Xfd; // ���ò�����9600
    TR1 = 1;    // ����ʱ��1�ж�
    SM0 = 0;    // ������ʽ1
    SM1 = 1;
    ES = 1;  // ��ʼ�����ж�
    ET0 = 1; // �򿪶�ʱ��0�ж�
    EA = 1;  // �����ж�
    TI = 1;
}
void SerialPushSend(uchar dat)
{
    SBUF = dat;
    while (!TI)
        ;
    TI = 0;
}
void delay(unsigned int k)
{
    unsigned int i, j;
    for (i = 0; i < k; i++)
    {
        for (j = 0; j < 121; j++)
            ;
    }
}
void Delay5us()
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}
void IIC_Start()
{
    SDA = 1;
    SCL = 1;
    Delay5us();
    SDA = 0;
    Delay5us();
    SCL = 0;
}
void IIC_Stop()
{
    SDA = 0;
    SCL = 1;
    Delay5us();
    SDA = 1;
    Delay5us();
}

void main()
{
    void Serial_init();
    while (1)
    {
    }
}
