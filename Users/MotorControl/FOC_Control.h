#ifndef __FOC_CONTROL_H
#define __FOC_CONTROL_H

#include "stm32f405xx.h"
#include "delay.h"
#include "arm_math.h"
#include "Coordinate_transform.h"
#include "Motor.h"
#include "PI_Cale.h"
#include "adc.h"
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "vofa.h"
#include "Sensorless_SMO.h"
#include "HFI.h"
#include "SVPWM.h"
#include "Filter.h"

typedef struct
{   //指令参数的斜率处理
    float  XieLv_X;   // 指令参数斜率输入变量x
    float  XieLv_Y;
    float  XieLv_Grad;
    u8    Timer_Count;
    u8    Grad_Timer;
} GXieLv, *p_GXieLv;

#define  GXieLv_DEFAULTS    {0,0,0,0,0}

extern 	CLARKE       CLARKE_PCurr;
extern 	PARK         PARK_PCurr;
extern  IPARK        IPARK_PVdq;
extern 	GXieLv       IF_Freq_GXieLv;
extern 	GXieLv			 Speed_Ref_GXieLv;
extern  IPARK				 IPARK_SMO_PVdq;

extern u8 					Fault_Code;			//故障代码
extern u8 					Cal_time;
extern vu8 					Speed_Cal_time;
extern float				IF_Theta;
extern bool 				Observe_Switch_flag;

void Grad_XieLv(p_GXieLv pv);				//梯度加减
void SVPWM_Cale(M_SVPWM pv);				//SVPWM调制算法函数
void Position_To_Zero(void);				//电机归零
void Fault_Detection(void);					//故障检测
void M_STOP(void);									//电机停止
void M_RUN(void);										//电机启动
void UVW_Axis_dq(void);							//坐标轴变换
void Speed_Closeloop(void);					//速度&电流闭环
void FOC_Svpwm_dq(void);						//SVPM计算
void QT_Motor(void);								//强制拖动
void FOC_Control_Selected(void);		//控制模式选择
void Control_Mode_CmdPara(void);  	// 控制模式运行
void Variable_Clear(void);					//变量清零
void Send_TO_PC(void);							//发送数据至上位机观测
void IF_Start_Control(void);				//IF启动
void Postion_To_Zero_Double(void);	//二次调零

#endif

