
/******************************************************************************/
/*----------------------------------引用---------------------------------------*/
/******************************************************************************/
#include "UserSource.h"
#include "ServeSource.h"
#include "VadcApp.c"
#include <stdio.h>
#include <stdlib.h>
int timecounter10=0;

/******************************************************************************/
/*---------------------------------用户变量定义-----------------------------------*/
/******************************************************************************/
uint8 ctldata=0;
int CodePerid;
float distance;
int i, j;
int arr_sum[8];
/******************************************************************************/
/*----------------------------------用户函数------------------------------------*/
/******************************************************************************/
//函数及函数用法
void motor_duty(int duty)//样例用户自身舵机函数
{
    if (duty>0)
        SetMotor(FORWARD, duty);
    else
        SetMotor(BACKWARD, -duty);
}
void steer_angle(int duty)
{
    if (duty>0)
        SetSteer(LEFT, duty);
    else if(duty<0)
        SetSteer(RIGHT, -duty);
    else
        SetSteer(MIDDLE, duty);
}

/*****************************滤波算法*********************************/
//对于电磁传感器的误差可以采取的滤波算法
int* avg_filter(void) // 5次测量取平均值
{
    // Read multiple set of data to get average values
    int arr_data[8] = {0,0,0,0,0,0,0,0}, i, *p;
    for (i = 0; i < 8; i ++)
    {
        arr_sum[i] = 0;
    }
    for (i = 0; i < 12; i ++)
    {
        VADCresult_run();
        for (j = 0; j < 8; j ++)
        {
            arr_sum[j] += VADCresult[j+1];
        }
    }
    for (i = 0; i < 8; i ++)
    {
        arr_sum[i] = arr_sum[i] / 12;
    }
    return arr_sum;
}

/*****************************电机驱动*********************************/
// 无PID,无特殊位置决策
void run(void)
{
    motor_duty(80); // 匀速80行驶
    avg_filter();
    // 正常行驶
    if (arr_sum[2] > arr_sum[3] + 1000) //转向阈值为1000
    {
        steer_angle(20);
    }
    else if (arr_sum[3] > arr_sum[2] + 1000)
    {
        steer_angle(-20);
    }
    else
    {
        steer_angle(0);
    }
    // 特殊位置
    sleep(50); //单次执行时间为100ms
}

/*****************************主函数***********************************/
//CPU0主函数，置于循环中用户主要逻辑计算区
void UserCpu0Main(void) //样例：蓝牙遥控小车
{
    while(1)
    {
        run();
    }
}
//CPU1主函数，置于循环中，摄像头读写由此核处理，建议用于摄像头相关计算：
//不要写成死循环，后面有AD相关处理
void UserCpu1Main(void)
{

}
/**************************************中断调用函数****************************************/
//该函数每10ms执行一次，请在该函数中书写程序，中断时间有限，不要太长
uint32 UserInterupt10ms(void)
{
    return 0;
}
//该函数每100ms执行一次，请在该函数中书写程序，中断时间有限，不要太长
//样例，获取编码器输出频率与超声举例
uint32 UserInterupt100ms(void)
{
    distance=get_echo_length();
    CodePerid=GetCodePerid();
    return 0;
}
//该函数每1000ms执行一次，请在该函数中书写程序，中断时间有限，不要太长
uint32 UserInterupt1000ms(void)
{
    return 0;
}

void UserInteruptIO(void)
{
    IfxPort_togglePin(LED1);
}
