//****************************************
//
// ע��MPU6050����оƬ���泯��
// ����: ��ʾ�Ƕ�ֵ
//****************************************
// GY-52 MPU6050 IIC���Գ���
// ʹ�õ�Ƭ��STC89C52
// ����11.0592M ��������19200
//****************************************
#include <REG52.H>
#include <math.h> //Keil library
// #include <stdio.h>   //Keil library
#include <INTRINS.H>
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
//****************************************
// ����51��Ƭ���˿�
//****************************************

sbit SCL = P2 ^ 2; // IICʱ�����Ŷ���
sbit SDA = P2 ^ 3; // IIC�������Ŷ���

//****************************************
// ����MPU6050�ڲ���ַ
//****************************************
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

�ڴ˻�����һ������˼�����⣺

//****************************************
// ����һ������ĵط�����������
// �����д�����жϺ��������º�������û�е��ã����ǲ���ɾ����һ��ɾ��������Ͳ����������С�
// ������Ϊ���˴����жϣ�����û��д�жϺ��������³���洢�ռ��д����ж���ڵ�ַ����������ռ�ã���ִ�д���ĳ���
// ���Դ��ж�ʱ������д����Ӧ���жϺ�������������������������
//****************************************

// ������
//*********************************************************

void main()
{
    int Ax, Ay, Az, Gx, Gy, Gz;
    unsigned long Gravity;
    float time, tosc;
    float AngleX1, AngleY1, AngleZ1, AngleX2, AngleY2, AngleZ2, dx, dy, dz;
    float Filter;
    Filter = 0.8; // �����˲�ϵ��
    delay(500);   // �ϵ���ʱ

    init_uart();
    InitMPU6050(); // ��ʼ��MPU6050
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

        // �������ʱ��
        TR0 = 0;
        time = TH0 << 8 + TL0;
        time = (float)(time + tn * 65536.0) * tosc;
        //        Show_int(tn);
        //        Show_int((int)(time*1000));
        TH0 = 0;
        TL0 = 0;
        tn = 0;
        TR0 = 1;

        // sqrt����������ϸ���float
        Gravity = sqrt((float)Ax * Ax + (float)Ay * Ay + (float)Az * Az); // Ax*Ax+Ay*Ay+Az*Az
        AngleX1 = acos((float)Ax / Gravity) * 180.0 / 3.14 - 90;
        ;
        AngleY1 = acos((float)Ay / Gravity) * 180.0 / 3.14 - 90;
        AngleZ1 = acos((float)Az / Gravity) * 180.0 / 3.14;
        dy = time * Gx / -16.4; // �����ǲ��ת��y
        dx = time * Gy / 16.4;  // �����ǲ��ת��x
        dz = time * Gz / 16.4;  // �����ǲ��ת��z

        // x��y���������ںϼ��ٶȼƺ�����������, z��ֻ��������������
        AngleX2 = Filter * (AngleX2 + dx) + (1 - Filter) * AngleX1;
        AngleY2 = Filter * (AngleY2 + dy) + (1 - Filter) * AngleY1;
        // z�����������ַ�ʽ��һ����ֻʹ�������ǵ����ݣ�����z����ٶȣ�z����Ʈ���أ���
        AngleZ2 = AngleZ2 + dz; // ע��MPU6050����оƬ���泯��

        // ��ʾ�Ƕ�
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
        SeriPushSend(0x0a); // ���У��س�
        ES = 0;
        while (RI == 0)
            ;
        RI = 0;
        ES = 1;
        //        delay(80);
    }
}