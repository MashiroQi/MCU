// Î»ÖÃÊ½pid
typedef struct
{
    float kp, ki, kd;
    float err, lastErr;
    float integral;
    float max;
    float min;
    float islimit;
    float dt;
} pidStruct;

pidStruct *pidInit()
{
    pidStruct *pid;
    pid->kp = 0.1;
    pid->ki = 0.2;
    pid->kd = 0.1;
    pid->err = 0;
    pid->lastErr = 0;
    pid->integral = 0;
    pid->islimit = true;
    pid->max = 0;
    pid->min = 180;
    pid->dt = 1;
    return pid;
}
float pid_realize(float target, float input, pidStruct *pid)
{
    float out;
    pid->err = target - input;
    pid->integral += pid->err;
    out = pid->kp * pid->err + pid->ki * pid->integral + pid->kd * (pid->err - pid->lastErr) / pid->dt;
    pid->lastErr = pid->err;
    if (pid->islimit)
    {
        if (out > pid->max)
            return pid->max;
        if (out < pid->min)
            return pid->min;
    }
    return out;
}
