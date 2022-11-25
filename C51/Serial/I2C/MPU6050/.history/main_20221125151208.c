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
unsigned char MPU6050ReadID(void)
{
    unsigned char Re = 0;
    MPU6050_ReadData(0x75, &Re, 1); // 读器件地址
    if (Re != 0x68)
    {
        MPU_ERROR("检测不到 MPU6050 模块");
        return 0;
    }
    else
    {
        MPU_INFO("MPU6050 ID = %d\r\n", Re);
        return 1;
    }
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