//****************************************
//
// 注意MPU6050必须芯片正面朝上
// 功能: 显示角度值
//****************************************
// GY-52 MPU6050 IIC测试程序
// 使用单片机STC89C52
// 晶振：11.0592M ，波特率19200
//****************************************
#include <REG52.H>
#include <math.h> //Keil library
// #include <stdio.h>   //Keil library
#include <INTRINS.H>
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
//****************************************
// 定义51单片机端口
//****************************************

sbit SCL = P2 ^ 2; // IIC时钟引脚定义
sbit SDA = P2 ^ 3; // IIC数据引脚定义

//****************************************
// 定义MPU6050内部地址
//****************************************
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

在此还发现一个有意思的问题：

//****************************************
// 发现一个神奇的地方！！！！！
// 如果不写串口中断函数，以下函数尽管没有调用，但是不能删除，一旦删除，程序就不能正常运行。
// 这是因为打开了串口中断，但是没有写中断函数，导致程序存储空间中串口中断入口地址被其他代码占用，而执行错误的程序。
// 所以打开中断时，必须写出对应的中断函数。！！！！！！！！！！
//****************************************

// 主程序
//*********************************************************

void main()
{
    int Ax, Ay, Az, Gx, Gy, Gz;
    unsigned long Gravity;
    float time, tosc;
    float AngleX1, AngleY1, AngleZ1, AngleX2, AngleY2, AngleZ2, dx, dy, dz;
    float Filter;
    Filter = 0.8; // 互补滤波系数
    delay(500);   // 上电延时

    init_uart();
    InitMPU6050(); // 初始化MPU6050
    delay(150);
    tosc = 12.0 / 11059200.0;
    AngleX1 = 0;
    AngleY1 = 0;
    AngleZ1 = 0;
    AngleX2 = 0;
    AngleY2 = 0;
    AngleZ2 = 0;
    dx = 0;
    dy = 0;
    dz = 0;

    while (1)
    {
        Ax = GetData(ACCEL_XOUT_H);
        Ay = GetData(ACCEL_YOUT_H);
        Az = GetData(ACCEL_ZOUT_H);
        Gx = GetData(GYRO_XOUT_H);
        Gy = GetData(GYRO_YOUT_H);
        Gz = GetData(GYRO_ZOUT_H);

        // 计算采样时间
        TR0 = 0;
        time = TH0 << 8 + TL0;
        time = (float)(time + tn * 65536.0) * tosc;
        //        Show_int(tn);
        //        Show_int((int)(time*1000));
        TH0 = 0;
        TL0 = 0;
        tn = 0;
        TR0 = 1;

        // sqrt的输入必须严格是float
        Gravity = sqrt((float)Ax * Ax + (float)Ay * Ay + (float)Az * Az); // Ax*Ax+Ay*Ay+Az*Az
        AngleX1 = acos((float)Ax / Gravity) * 180.0 / 3.14 - 90;
        ;
        AngleY1 = acos((float)Ay / Gravity) * 180.0 / 3.14 - 90;
        AngleZ1 = acos((float)Az / Gravity) * 180.0 / 3.14;
        dy = time * Gx / -16.4; // 陀螺仪测的转角y
        dx = time * Gy / 16.4;  // 陀螺仪测的转角x
        dz = time * Gz / 16.4;  // 陀螺仪测的转角z

        // x和y轴数据是融合加速度计和陀螺仪数据, z轴只采用陀螺仪数据
        AngleX2 = Filter * (AngleX2 + dx) + (1 - Filter) * AngleX1;
        AngleY2 = Filter * (AngleY2 + dy) + (1 - Filter) * AngleY1;
        // z轴数据有两种方式，一种是只使用陀螺仪的数据，舍弃z轴加速度（z轴零飘严重）：
        AngleZ2 = AngleZ2 + dz; // 注意MPU6050必须芯片正面朝上

        // 显示角度
        SeriPushSend('x');
        SeriPushSend(':');
        Show_int((int)(AngleX2));
        SeriPushSend('y');
        SeriPushSend(':');
        Show_int((int)(AngleY2));
        SeriPushSend('z');
        SeriPushSend(':');
        Show_int((int)(AngleZ2));

        SeriPushSend(0x0d);
        SeriPushSend(0x0a); // 换行，回车
        ES = 0;
        while (RI == 0)
            ;
        RI = 0;
        ES = 1;
        //        delay(80);
    }
}