#include <REGX52.H>
#include <stdio.h>
#include <INTRINS.H>
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
sbit SCL = P1 ^ 5;        // IIC时钟引脚定义
sbit SDA = P1 ^ 4;        // IIC数据引脚定义
#define SMPLRT_DIV 0x19   // 陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG 0x1A       // 低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG 0x1B  // 陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG 0x1C // 加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
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
#define PWR_MGMT_1 0x6B   // 电源管理，典型值：0x00(正常启用)
#define WHO_AM_I 0x75     // IIC地址寄存器(默认数值0x68，只读)
#define SlaveAddress 0xD0 // IIC写入时的地址字节数据，+1为读取

uchar dis[6]; // 显示数字(-511至512)的字符数组
int dis_data; // 变量

void Serial_init()
{
    TMOD |= 0X20;
    TH1 = 0Xfd;
    TL1 = 0Xfd; // 设置波特率9600
    TR1 = 1;    // 开定时器1中断
    SM0 = 0;    // 工作方式1
    SM1 = 1;
    ES = 1;  // 开始串口中断
    ET0 = 1; // 打开定时器0中断
    EA = 1;  // 开总中断
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
void IIC_SendAck(bit ack)
{
    SDA = ack;
    SCL = 1;
    Delay5us();
    SCL = 0;
    Delay5us();
}
void IIC_RecAck()
{
    SCL = 1;
    Delay5us();
    CY = SDA;
    SCL = 0;
    Delay5us();
    return;
}
void IIC_SendByte(uchar dat)
{
    uchar i;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        SDA = CY;
        SCL = 1;
        Delay5us();
        SCL = 0;
        Delay5us();
    }
    IIC_RecAck();
}
uchar IIC_RecByte()
{
    uchar i;
    uchar dat = 0;
    SDA = 1;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        SCL = 1;
        Delay5us();
        dat |= SDA;
        SCL = 0;
        Delay5us();
    }
    return dat;
}
void IIC_Write(uchar REG_Addr, uchar REG_dat)
{
    IIC_Start();
    IIC_SendByte(SlaveAddress);
    IIC_SendByte(REG_Addr);
    IIC_SendByte(REG_dat);
    IIC_Stop();
}
uchar IIC_Read(uchar REG_Addr)
{
    uchar rec_dat;
    IIC_Start();
    IIC_SendByte(SlaveAddress);
    IIC_SendByte(REG_Addr);
    IIC_Start();
    IIC_SendByte(SlaveAddress + 1);
    rec_dat = IIC_RecByte();
    IIC_SendAck(1);
    IIC_Stop();
}
void InitMPU6050()
{
    IIC_Write(PWR_MGMT_1, 0x00); // 解除休眠状态
    IIC_Write(SMPLRT_DIV, 0x07);
    IIC_Write(CONFIG, 0x06);
    IIC_Write(GYRO_CONFIG, 0x18);
    IIC_Write(ACCEL_CONFIG, 0x01);
}
int GetData(uchar REG_Address)
{
    uchar H, L;
    H = IIC_Read(REG_Address);
    L = IIC_Read(REG_Address + 1);
    return ((H << 8) + L); // 合成数据
}
void main()
{
    Serial_init();
    InitMPU6050();
    while (1)
    {
        printf("%d ", GetData(ACCEL_XOUT_H)); // 显示X轴加速度
        printf("%d ", GetData(ACCEL_YOUT_H)); // 显示Y轴加速度
        printf("%d ", GetData(ACCEL_ZOUT_H)); // 显示Z轴加速度
        printf("%d ", GetData(GYRO_XOUT_H));  // 显示X轴角速度
        printf("%d ", GetData(GYRO_YOUT_H));  // 显示Y轴角速度
        printf("%d\n", GetData(GYRO_ZOUT_H)); // 显示Z轴角速度
    }
}
