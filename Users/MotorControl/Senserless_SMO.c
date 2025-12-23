#include "FOC_Control.h"

#define wc					BWc_rps*PIX2			//wc=截止频率*2Pi

Angle_SMO 	Angle_SMO_Para=Angle_SMO_DEFAULTS;
PLL					PLL_SMO_Para=PLL_DEFAULTS;
PLL					PLL_HFI_Para=PLL_DEFAULTS;

float SMO_theta_compersation=0;			//角度补偿
float SMO_Angle1;										//角度补偿后的角度
//滑膜观测器角度计算
void SMO_Angle_Cale(p_Angle_SMO pv)
{
//	pv->Ialpha = CLARKE_PCurr.Alpha;
//	pv->Ibeta = CLARKE_PCurr.Beta;
//
//	pv->Ualpha = SVPWM_dq.Ualpha;
//	pv->Ubeta = SVPWM_dq.Ubeta;
    //观测电流
    pv->Est_Ialpha += TS*(-Motor.Rs/Motor.Ls*pv->Est_Ialpha + 1/Motor.Ls*(pv->Ualpha - pv->Zalpha));
    pv->Est_Ibeta += TS*(-Motor.Rs/Motor.Ls*pv->Est_Ibeta + 1/Motor.Ls*(pv->Ubeta -pv->Zbeta));
    //反电动势，Zalpha与Zbeta是开关信号，需要经过滤波才能得到反电动势的等效值
//	pv->Zalpha = SMO_Kslide*Sign(pv->Est_Ialpha - pv->Ialpha);
//	pv->Zbeta = SMO_Kslide*Sign(pv->Est_Ibeta - pv->Ibeta);
    pv->Zalpha = SMO_Kslide*Sat(pv->Est_Ialpha - pv->Ialpha, 0.5f);
    pv->Zbeta = SMO_Kslide*Sat(pv->Est_Ibeta - pv->Ibeta, 0.5f);
    //低通滤波
    pv->Ealpha = pv->Ealpha*LPF_SMO_B + pv->Zalpha*LPF_SMO_A;
    pv->Ebeta = pv->Ebeta*LPF_SMO_B + pv->Zbeta*LPF_SMO_A;

//	pv->atan_theta =my_atan2(pv->Ebeta, -pv->Ealpha);						//反正切法获得角度
//	SMO_theta_compersation = my_atan(PLL_HFI_Para.Omega/(-wc));
//	SMO_Angle1 = pv->atan_theta - SMO_theta_compersation;
//	if(SMO_Angle1 >= PIX2)
//		SMO_Angle1 -=PIX2;
//	else if(SMO_Angle1 < 0)
//		SMO_Angle1 += PIX2;
    if(Sensorless.Speed_Ref >= 0)
        PLL_Cale((p_PLL)&PLL_SMO_Para, pv->Ealpha, pv->Ebeta);			//通过PLL获得角度与速度
    else
        PLL_Cale((p_PLL)&PLL_SMO_Para, -pv->Ealpha, -pv->Ebeta);
}


void SMO_Init(void)
{
    Angle_SMO_Para.Fsmopos = exp((-Motor.Rs/Motor.Ls)*TS);
    Angle_SMO_Para.Gsmopos = (Motor.Vb/Motor.Ib)*(1/Motor.Rs)*(1 - Angle_SMO_Para.Fsmopos);
    Angle_SMO_Para.E0 = 0.5;					//标幺值1的一半
}


//void SMO_Angle_Cale(p_Angle_SMO pv)
//{
//	float IalphaError_Limit=0,IbetaError_Limit=0;
//	/*	Sliding mode current observer	*/
//	pv->Est_Ialpha = pv->Est_Ialpha*pv->Fsmopos + (pv->Ualpha - pv->Ealpha - pv->Zalpha)*pv->Gsmopos;
//	pv->Est_Ibeta = pv->Est_Ibeta*pv->Fsmopos + (pv->Ubeta - pv->Ebeta - pv->Zbeta)*pv->Gsmopos;
//
//	/*	Current errors	*/
//  pv->Ialpha_Err = pv->Est_Ialpha - pv->Ialpha;
//	pv->Ibeta_Err = pv->Est_Ibeta - pv->Ibeta;
//
//	/*  Sliding control calculator	*/
//	/* pV->Zalpha=pV->IalphaError*pV->Kslide/pV->E0) where E0=0.5 here*/
//	//限制赋值函数
//	IalphaError_Limit = Limit_Sat(pv->Ialpha_Err, pv->E0, -pv->E0);
//	IbetaError_Limit = Limit_Sat(pv->Ibeta_Err, pv->E0, -pv->E0);
//
//	pv->Zalpha = IalphaError_Limit*SMO_Kslide;
//	pv->Zbeta = IbetaError_Limit*SMO_Kslide;
//
//	/*	Sliding control filter -> back EMF calculator	*/
//	pv->Ealpha = pv->Ealpha + (pv->Zalpha - pv->Ealpha)*SMO_Kslf;
//	pv->Ebeta = pv->Ebeta + (pv->Zbeta - pv->Ebeta)*SMO_Kslf;
//
////	pv->atan_theta =my_atan2(pv->Ebeta, -pv->Ealpha);
//	PLL_Cale((p_PLL)&PLL_SMO_Para, pv->Ealpha, pv->Ebeta);
//}

//正交锁相环
void PLL_Cale(p_PLL pv, float alpha, float beta)
{
    pv->Theta_Err = -(alpha*arm_cos_f32(pv->Theta)) + (-beta)*arm_sin_f32(pv->Theta);					//通过矢量叉乘或者角度误差
    pv->Omega += pv->Kp*(pv->Theta_Err - pv->Theta_Err_last) + TS*pv->Ki*pv->Theta_Err;				//经过PI调节器获得转速，转速为弧度制电角速度
//	pv->Omega_F = pv->Omega_F*LPF_PLL_B + pv->Omega*LPF_PLL_A;
    if(Sensorless.Observer == 3)
        pv->Omega_F = pv->Omega_F*LPF_PLL_B + pv->Omega*LPF_PLL_A;
    else
        IIR_Butterworth(PLL_HFI_Para.Omega, &PLL_HFI_Para.Omega_F, &WE_IIR_LPF_Par);
    pv->Theta_Err_last = pv->Theta_Err;
    pv->Theta += TS*pv->Omega;																																//对电角速度进行积分获得电角度
    //对电角度进行限幅
    if(pv->Theta >= PIX2)
        pv->Theta -= PIX2;
    else if(pv->Theta < 0)
        pv->Theta += PIX2;
    //在SMO中，反电动势需要进行LPF，因此存在滞后，通过观测电角速度对电角度进行补偿
    SMO_theta_compersation = my_atan(my_abs(pv->Omega)/(-wc));
    pv->Angle = pv->Theta - SMO_theta_compersation;
    //对补偿后的电角度进行限幅
    if(pv->Angle >= PIX2)
        pv->Angle -= PIX2;
    else if(pv->Angle < 0)
        pv->Angle += PIX2;
}

//SMO速度闭环控制
void SMO_Observer(float Id, float Iq)
{
    float outMax_V;
    float	angleDelta_rad;
    float angleFoc_rad;

    if(Speed_Cal_time == Speed_Cal_Period)
    {
        pi_spd.Ref = Sensorless.Speed_Ref;
        pi_spd.Fbk = Sensorless.Speed_Fbk;
        PI_Controller((M_PI_Control)&pi_spd);									//速度环
        pi_spd.OutF = pi_spd.OutF*LPF_I_RUN_B + pi_spd.Out*LPF_I_RUN_A; //环路滤波后输出
    }

    pi_id.Ref = 0;																				//id=0，d轴电流闭环，力矩大
    pi_id.Fbk = Id;
    PI_Controller((M_PI_Control)&pi_id);									//Id轴闭环
    pi_id.OutF = pi_id.OutF*LPF_I_RUN_B + pi_id.Out*LPF_I_RUN_A;

    //防止过调制
    float maxVsMag_V = MaxVsMagPu * ADC_Sample_Filt_Para.VBUS;
    outMax_V = maxVsMag_V * maxVsMag_V -  pi_id.OutF *  pi_id.OutF;
    arm_sqrt_f32(outMax_V, &outMax_V);

    //Iq轴闭环
    pi_iq.Umax = 	outMax_V;
    pi_iq.Umin = -outMax_V;
    pi_iq.Ref = pi_spd.OutF;																			//Iq的设定和电机的额定转矩有关，IF启动一般Iq选择比额定转矩下的Iq略大
    pi_iq.Fbk	= Iq;
    PI_Controller((M_PI_Control)&pi_iq);									//Iq轴闭环
    pi_iq.OutF = pi_iq.OutF*LPF_I_RUN_B + pi_iq.Out*LPF_I_RUN_A;


    //用延时补偿计算角度
    angleDelta_rad = TS*PLL_SMO_Para.Omega_F;
    angleFoc_rad = MATH_incrAngle(Sensorless.theta, angleDelta_rad);
		
//    IPARK_PVdq.Theta = Sensorless.theta;
		IPARK_PVdq.Theta = angleFoc_rad;
    IPARK_PVdq.Ds = pi_id.OutF;
    IPARK_PVdq.Qs = pi_iq.OutF;
    IPARK_Cale((M_IPARK)&IPARK_PVdq);											//IPARK函数计算
}

//滑膜观测器角度计算
void SMO_Theta_Cale(float Ialpha, float Ibeta, float Ualpha, float Ubeta)
{
    Angle_SMO_Para.Ialpha = Ialpha;
    Angle_SMO_Para.Ibeta = Ibeta;
    Angle_SMO_Para.Ualpha = Ualpha;
    Angle_SMO_Para.Ubeta = Ubeta;
    SMO_Angle_Cale((p_Angle_SMO)&Angle_SMO_Para);
}
