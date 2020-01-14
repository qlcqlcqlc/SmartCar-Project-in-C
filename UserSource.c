
/******************************************************************************/
/*----------------------------------引用---------------------------------------*/
/******************************************************************************/
#include "UserSource.h"
#include "ServeSource.h"
#include <stdio.h>
int timecounter10=0;

/******************************************************************************/
/*---------------------------------用户变量定义-----------------------------------*/
/******************************************************************************/
uint8 ctldata=0;
int CodePerid;
float distance;
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
void average_filter(void)
{

}

/*****************************电磁采集*********************************/
//磁场数据采样

VADC_init(void);

void VADCresult_run(void)
{
    int i, j;
    int arr[9][9];
    for (i = 0; i < 9; i ++)
    {
        for (j = 0; j < 9; j ++)
        {
            arr[i][j] = 0;
        }
    }
    // wait for valid result
    Ifx_VADC_RES conversionResult;
    // start scan
    IfxVadc_Adc_startScan(&adcGroup);
    // check results
    int chnIx;
    i = 0;
    while (i < 9)
    {
        for(chnIx=0; chnIx<9; chnIx++) {
            do {
                conversionResult = IfxVadc_Adc_getResult(&adcChannel[chnIx]);
            } while( !conversionResult.B.VF );
            VADCresult[chnIx]=conversionResult.B.RESULT;
            arr[i][chnIx] = VADCresult[chnIx];
        }
        i ++;
        sleep(10);
    }
    for (i = 0; i < 9; i ++)
    {
        for (j = 0; j < 9; j ++)
        {
            printf("%d ", arr[i][j]);
        }
        printf('\n');
    }
    printf("The End of Result");
}


/*****************************主函数***********************************/
//CPU0主函数，置于循环中用户主要逻辑计算区
void UserCpu0Main(void) //样例：蓝牙遥控小车
{
    uint8 a=0;
    int myduty=0,myangle=0;
    motor_duty(myduty);
    steer_angle(myangle);
    while(TRUE)
    {
        a=Bluetooth_Read_Data();
        if (a!=0)
            ctldata=a;
        switch (ctldata)
        {
            case 'W': // 大写W为喷射起步
                myduty=80;
                ctldata='W';
                motor_duty(myduty);
                steer_angle(myangle);
                Bluetooth_Send_Data(ctldata);
                break;
            case 'w': // 速度20渐增
                myduty+=20;
                ctldata='w';
                motor_duty(myduty);
                steer_angle(myangle);
                Bluetooth_Send_Data(ctldata);
                break;
            case 's': // 速度20渐减
                myduty-=20;
                ctldata='s';
                motor_duty(myduty);
                steer_angle(myangle);
                Bluetooth_Send_Data(ctldata);
                break;
            case 'a': // 角度增加20
                myangle+=20; // 无复位操作！！！
                ctldata='a';
                motor_duty(myduty);
                steer_angle(myangle);
                Bluetooth_Send_Data(ctldata);
                break;
            case 'd': // 角度减少20
                myangle-=20; // 无复位操作！！！
                ctldata='d';
                motor_duty(myduty);
                steer_angle(myangle);
                Bluetooth_Send_Data(ctldata);
                break;
            case '/': // 急刹车
                myduty=0;
                ctldata='W';
                motor_duty(myduty);
                steer_angle(myangle);
                Bluetooth_Send_Data(ctldata);
                break;

                //可以把Q和E设置为特定半径圆的自动转圈
        }

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
