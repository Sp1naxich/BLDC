#ifndef __SENSORLESS_SMO_H
#define __SENSORLESS_SMO_H

#include "main.h"
#include "delay.h"

#define LPF_SMO_A		0.2199f				//低通滤波系数，截止频率700Hz
#define LPF_SMO_B		0.7801f

#define LPF_PLL_A		0.09425f			//锁相环速度低通滤波系数，截止频率300Hz	
#define LPF_PLL_B		0.90575f

#define SMO_Kslide	1.868f				//滑膜增益K
#define SMO_Kslf		0.12566f			//低通滤波系数

typedef struct {
    float Ualpha;									//两相静止坐标系α轴电压
    float Ubeta;									//两相静止坐标系β轴电压
    float Ialpha;									//两相静止坐标系α轴电流
    float Ibeta;									//两相静止坐标系β轴电流
    float Est_Ialpha;							//两相静止坐标系观测的α电流
    float Est_Ibeta;							//两相静止坐标系观测的β电流
    float Ialpha_Err;							//两相静止坐标系α轴电流误差
    float Ibeta_Err;							//两相静止坐标系β轴电流误差
    float Zalpha;									//滑膜观测器的α轴Z平面
    float Zbeta;									//滑膜观测器的β轴Z平面
    float Ealpha;									//滑膜观测的α轴反电动势
    float Ebeta;									//滑膜观测的β轴反电动势
    float Fsmopos;								//滑膜系数1
    float Gsmopos;								//滑膜系数2
    float E0;											//滑膜的电流误差的限幅值 0.5
    float atan_theta;							//反正切计算的角度
} Angle_SMO, *p_Angle_SMO;

#define Angle_SMO_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}			//初始化参数

typedef struct {
    float 	Kp;										//锁相环PI控制器Kp
    double 	Ki;										//锁相环PI控制器Ki
    float 	Theta_Err;						//角度误差
    float 	Theta_Err_last;				//上一次角度误差
    float 	Omega;								//电角速度
    float 	Omega_F;							//滤波后的电角速度
    float 	Theta;								//锁相环计算得到的电角度
    float 	Angle;								//经过角度补偿的电角度
    float 	Sum;
} PLL, *p_PLL;

#define PLL_DEFAULTS {0,0,0,0,0,0,0,0,0};														//初始化参数

extern Angle_SMO		Angle_SMO_Para;
extern PLL					PLL_SMO_Para;
extern PLL					PLL_HFI_Para;

extern float SMO_Angle1;

void SMO_Angle_Cale(p_Angle_SMO pv);									//滑膜观测器
void PLL_Cale(p_PLL pv,float alpha, float beta);			//通过锁相环获得观测电角度和速度
void SMO_Init(void);																	//滑膜参数初始化
void SMO_Observer(float Id, float Iq);
void SMO_Theta_Cale(float Ialpha, float Ibeta, float Ualpha, float Ubeta);

#endif

