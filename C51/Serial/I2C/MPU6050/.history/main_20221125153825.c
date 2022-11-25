#include <REG52.H>
#include <stdio.h>
sbit SCL = P2 ^ 0;
sbit SDA = P2 ^ 1;
void MPU6050ReadGyro(short *gyroData);
void MPU6050_Init(void);
void MPU6050ReadAcc(short *accData);
void MPU6050ReadGyro(short *gyroData);
void MPU6050_ReadData(unsigned char reg_add, unsigned char *Read, unsigned char num);
void Serial_init();
void main()
{
    short Accel[3];
    short Gyro[3];
    Serial_init();
    MPU6050_Init();
    while (1)
    {

        MPU6050ReadAcc(Accel);
        printf("加速度 %8d%8d%8d\n", Accel[0], Accel[1], Accel[2]);
        MPU6050ReadGyro(Gyro);
        printf("陀螺仪 %8d%8d%8d\n", Gyro[0], Gyro[1], Gyro[2]);
        delay(10000);
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
    delay(10);
    SDA = 0;
    delay(10);
    SCL = 0;
}
void IIC_Stop()
{
    SDA = 0;
    SCL = 1;
    delay(10);
    SDA = 1;
}

unsigned char IIC_Wait_Ack()
{
    unsigned char re;
    SDA = 1;
    delay(10);
    SCL = 1;
    delay(10);
    if (SDA)
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    SCL = 0;
    delay(10);
    return re;
}
void Ack()
{
    SDA = 0;
    delay(10);
    SCL = 1;
    delay(10);
    SCL = 0;
    delay(10);
    SDA = 1;
}
void NoAck()
{
    SDA = 1;
    delay(10);
    SCL = 1;
    delay(10);
    SCL = 0;
    delay(10);
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
        delay(10);
        SCL = 1;
        delay(10);
        SCL = 0;
        if (i == 7)
        {
            SDA = 1;
        }
        dat <<= 1;
        delay(10);
    }
}

unsigned char IIC_Read_Byte(unsigned char ask)
{
    unsigned char i = 0;
    unsigned char dat;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        SCL = 1;
        delay(10);
        if (SDA == 1)
        {
            dat += 1;
        }
    }
    if (ask)
    {
        Ack();
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

void MPU6050ReadGyro(short *gyroData)
{
    unsigned char buf[6];
    MPU6050_ReadData(0x43, buf, 6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}
void MPU6050ReadAcc(short *accData)
{
    unsigned char buf[6];
    MPU6050_ReadData(0x3B, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

void Serial_init()
{
    TMOD |= 0X20;
    TH1 = 0Xfd;
    TL1 = 0Xfd; // 设置波特率9600
    TR1 = 1;    // 开定时器1中断
    SM0 = 0;    // 工作方式1
    SM1 = 1;
    ES = 1; // 开始串口中断
    EA = 1; // 开总中断
    TI = 1;
}

void MPU6050_Init(void)
{
    int i = 0, j = 0;
    for (i = 0; i < 1000; i++)
    {
        for (j = 0; j < 1000; j++)
            ;
    }
    IIC_WriteByte(0x6b, 0x01);
    IIC_WriteByte(0x19, 0x07);
    IIC_WriteByte(0x1a, 0x03);
    IIC_WriteByte(0x1c, 0x00);
    IIC_WriteByte(0x1b, 0x18);
}