#include "FOC_Control.h"

HFI_Cal				HFI = HFI_DEFAULTS;

//HFI角度计算
void HFI_Angle_Cale(p_HFI pv)
{
	//提取高频电流分量
	pv->ialpha_h_last = pv->ialpha_h;
	pv->ibeta_h_last = pv->ibeta_h;

	if(HFI_Filter_flag)
	{
		IIR_Butterworth(CLARKE_PCurr.Alpha, &pv->ialpha_h, &A_IIR_HPF_Par);
		IIR_Butterworth(CLARKE_PCurr.Beta, &pv->ibeta_h, &B_IIR_HPF_Par);

		IIR_Butterworth(CLARKE_PCurr.Alpha, &pv->ialpha_f, &A_IIR_LPF_Par);
		IIR_Butterworth(CLARKE_PCurr.Beta, &pv->ibeta_f, &B_IIR_LPF_Par);
	}
	else
	{
		pv->ialpha_h = (CLARKE_PCurr.Alpha - pv->Ialpha_last)*0.5f;
		pv->ibeta_h = (CLARKE_PCurr.Beta - pv->Ibeta_last)*0.5f;

		pv->ialpha_f = (CLARKE_PCurr.Alpha + pv->Ialpha_last)*0.5f;
		pv->ibeta_f = (CLARKE_PCurr.Beta + pv->Ibeta_last)*0.5f;

		pv->Ialpha_last = CLARKE_PCurr.Alpha;
		pv->Ibeta_last = CLARKE_PCurr.Beta;
	}

	pv->SIGN = -Sign(pv->Uin);

	//包络检测
	pv->Ialpha_h = (pv->ialpha_h - pv->ialpha_h_last)*pv->SIGN;
	pv->Ibeta_h = (pv->ibeta_h - pv->ibeta_h_last)*pv->SIGN;

	//	pv->theta = my_atan2(pv->Ialpha_h, pv->Ibeta_h);
	PLL_Cale((p_PLL)&PLL_HFI_Para, -pv->Ibeta_h, pv->Ialpha_h);
}

//脉振高频注入角度计算
float hfi_sin;
float hfi_cos;
//bool 	HFI_inj_volt_amp_ok;
void HFI_P_Angle_Cale(p_HFI pv)
{
	float temp;
	//	float hfi_pid_out;

	//提取角度误差
	temp = pv->Iqh*hfi_sin;
	IIR_Butterworth(temp, &PLL_HFI_Para.Theta_Err, &ERR_IIR_LPF_Par);				//过低通滤波

	//PLL，增量式PID
	PLL_HFI_Para.Omega += PLL_HFI_Para.Kp*(PLL_HFI_Para.Theta_Err - PLL_HFI_Para.Theta_Err_last) + TS*PLL_HFI_Para.Ki*PLL_HFI_Para.Theta_Err;
	PLL_HFI_Para.Omega_F = PLL_HFI_Para.Omega_F*LPF_PLL_B +PLL_HFI_Para.Omega*LPF_PLL_A;
	//	IIR_Butterworth(PLL_HFI_Para.Omega, &PLL_HFI_Para.Omega_F, &WE_IIR_LPF_Par);
	PLL_HFI_Para.Theta_Err_last = PLL_HFI_Para.Theta_Err;

	PLL_HFI_Para.Theta += TS*PLL_HFI_Para.Omega;																																//对电角速度进行积分获得电角度
	//对电角度进行限幅
	if(PLL_HFI_Para.Theta >= PIX2)
		PLL_HFI_Para.Theta -= PIX2;
	else if(PLL_HFI_Para.Theta < 0)
		PLL_HFI_Para.Theta += PIX2;

	//PLL, 增量式
	//		hfi_pid_out = 50*PLL_HFI_Para.Theta_Err + PLL_HFI_Para.Sum;
	//		PLL_HFI_Para.Theta = PLL_HFI_Para.Theta_Err_last;
	//		PLL_HFI_Para.Omega = hfi_pid_out;
	//		PLL_HFI_Para.Theta_Err_last += TS*hfi_pid_out;
	//
	//		if(PLL_HFI_Para.Theta_Err_last < 0.0f)
	//			PLL_HFI_Para.Theta_Err_last += PIX2;
	//		else if(PLL_HFI_Para.Theta_Err_last > PIX2)
	//			PLL_HFI_Para.Theta_Err_last -= PIX2;
	//
	//		PLL_HFI_Para.Sum += (500*PLL_HFI_Para.Theta_Err) * TS;
}

//高频方波注入初始角度识别
bool HFI_Uin_flag;
u8 	 HFI_Fre_CNT;
void HFI_Injection_Fre(void)
{
	HFI_Fre_CNT ++;

	if(HFI_Fre_CNT == HFI_SW_Fre)
	{
		//			 LED3 =! LED3;
		HFI_Fre_CNT = 0;
		HFI_Uin_flag = !HFI_Uin_flag;
	}
}

void HFI_Init_Theta(void)
{
	static u16 		HFI_Clock = 0;
	static float 	Sum_Id1 = 0;
	static float 	Sum_Id2 = 0;

	if(HFI_Init_Angle_flag == 0)
		HFI_Clock ++;

	//    HFI_Uin_flag = !HFI_Uin_flag;
	HFI_Injection_Fre();

	if(HFI_Filter_flag)
	{
		//提取低频电流分量
		IIR_Butterworth(PARK_PCurr.Ds, &HFI.Idf, &D_IIR_LPF_Par);
		IIR_Butterworth(PARK_PCurr.Qs, &HFI.Iqf, &Q_IIR_LPF_Par);
		//提取高频电流分量
		IIR_Butterworth(PARK_PCurr.Ds, &HFI.Idh, &D_IIR_HPF_Par);
	}
	else
	{
		//提取低频电流分量
		HFI.Idf = (PARK_PCurr.Ds + HFI.Id_last)*0.5f;
		HFI.Iqf = (PARK_PCurr.Qs + HFI.Iq_last)*0.5f;
		//提取高频电流分量
		HFI.Idh = (PARK_PCurr.Ds - HFI.Id_last)*0.5f;

		HFI.Id_last = PARK_PCurr.Ds;
		HFI.Iq_last = PARK_PCurr.Qs;
	}

	if(HFI_Clock < 400)									//20ms，识别的初始电角度收敛
	{
		pi_id.Ref = 0;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}
	else if(HFI_Clock == 400)						//初始电角度赋值
	{
		HFI.theta_Init = PLL_HFI_Para.Theta;		//初始角度赋值
		pi_id.Ref = 0;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}
	else if((HFI_Clock > 400) && (HFI_Clock <= 600))			//10ms,Id偏置电流注入
	{
		pi_id.Ref = HFI_Id_offset;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}
	else if((HFI_Clock > 600) && (HFI_Clock<=610))			//累计 高频电流分量*Id_offset
	{
		pi_id.Ref = HFI_Id_offset;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
		Sum_Id1 += my_abs(HFI.Idh*HFI_Id_offset);
	}
	else if((HFI_Clock > 610) && (HFI_Clock <= 800))		//10ms,Id偏执电流归零
	{
		pi_id.Ref = 0;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}
	else if((HFI_Clock > 800) && (HFI_Clock <= 1000))		//10ms,Id负偏置电流注入
	{
		pi_id.Ref = -HFI_Id_offset;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}
	else if((HFI_Clock > 1000) && (HFI_Clock <= 1010))		//累计 高频电流分量*Id_offset
	{
		pi_id.Ref = -HFI_Id_offset;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
		Sum_Id2 += my_abs(-HFI.Idh*HFI_Id_offset);
	}
	else if(HFI_Clock > 1010)															//初始角度识别结束，根据Sum_Id1&2大小判断收敛在N极还是S极
	{
		HFI_Init_Angle_flag = 1;
		if(Sum_Id1 < Sum_Id2)																//收敛在S极，对角度进行补偿
		{
			HFI.theta_Init = PLL_HFI_Para.Theta + PI;
			if(HFI.theta_Init > PIX2)
				HFI.theta_Init -= PIX2;

			//			HFI.theta = HFI.theta_Init;
			PLL_HFI_Para.Theta = HFI.theta_Init;
		}
		Sum_Id1 = 0;
		Sum_Id2 = 0;

		pi_id.Ref = 0;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
		HFI_Clock = 0;
	}

	//注入的高频方波电压
	if(HFI_Uin_flag)
		HFI.Uin = HFI_Uin_offset;
	else
		HFI.Uin = -HFI_Uin_offset;

	//IPARK计算
	IPARK_PVdq.Theta = PLL_HFI_Para.Theta;
	IPARK_PVdq.Qs = pi_iq.Out;
	IPARK_PVdq.Ds = pi_id.Out + HFI.Uin;				//d轴高频方波注入
	IPARK_Cale((M_IPARK)&IPARK_PVdq);
}

float HFI_P_t = 0;
s16 	hfi_cnt;

//脉振高频注入初始角度识别
void HFI_P_Init_Theta(void)
{
	static u16	 HFI_Clock = 0;
	static float Sum_Id1 = 0;
	static float Sum_Id2 = 0;
	//    static float id_max;
	//    static float id_min;
	//    static u16   d_direction_ok_cnt1;
	//    static u16 	 d_direction_ok_cnt2;
	//    static u8 	 id_index;
	float 		   temp;

	if(HFI_Init_Angle_flag == 0)
		HFI_Clock ++;

	if(hfi_cnt > HFI_CNT_FLAG)
		hfi_cnt = 0;

	temp = hfi_cnt;

	hfi_cos = arm_cos_f32((temp/HFI_CNT_FLAG)*PIX2);
	hfi_sin = arm_sin_f32((temp/HFI_CNT_FLAG)*PIX2);

	//提取低频电流分量,低通滤波
	IIR_Butterworth(PARK_PCurr.Ds, &HFI.Idf, &D_IIR_LPF_Par);
	IIR_Butterworth(PARK_PCurr.Qs, &HFI.Iqf, &Q_IIR_LPF_Par);

	//提取高频电流分量
	arm_iir_f32_bpf(&PARK_PCurr.Qs, &HFI.Iqh);
	//		arm_iir_f32_bpf(&PARK_PCurr.Ds, &HFI.Idh);
	IIR_Butterworth(PARK_PCurr.Ds, &HFI.Idh, &D_IIR_BPF_Par);


	if(HFI_Clock < 400)									//20ms，识别的初始电角度收敛
	{
		pi_id.Ref = 0;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}
	else if(HFI_Clock == 400)									//初始电角度赋值,此处HFI_Clock到400即20ms后，启动磁极极性辨识
	{
		HFI.theta_Init = PLL_HFI_Para.Theta;		//初始角度赋值
		pi_id.Ref = 0;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//        HFI_inj_volt_amp_ok = 1;								//磁极极性辨识开启标志位
		//        HFI_Clock = 0;

		//		HFI_Init_Angle_flag = 1;

		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}

	else if((HFI_Clock > 400) && (HFI_Clock <= 600))			//10ms,Id偏置电流注入
	{
		pi_id.Ref = HFI_Id_offset;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);

		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}
	else if((HFI_Clock > 600) && (HFI_Clock<=650))			//累计 高频电流分量*Id_offset
	{
		pi_id.Ref = HFI_Id_offset;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
		Sum_Id1 += my_abs(HFI.Idh*HFI_Id_offset);
	}
	else if((HFI_Clock > 650) && (HFI_Clock <= 800))		//10ms,Id偏执电流归零
	{
		pi_id.Ref = 0;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}
	else if((HFI_Clock > 800) && (HFI_Clock <= 1000))		//10ms,Id负偏置电流注入
	{
		pi_id.Ref = -HFI_Id_offset;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	}
	else if((HFI_Clock > 1000) && (HFI_Clock <= 1050))		//累计 高频电流分量*Id_offset
	{
		pi_id.Ref = -HFI_Id_offset;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
		Sum_Id2 += my_abs(HFI.Idh*HFI_Id_offset);
	}
	else if(HFI_Clock > 1050)															//初始角度识别结束，根据Sum_Id1&2大小判断收敛在N极还是S极
	{
		HFI_Init_Angle_flag = 1;
		if(Sum_Id1 < Sum_Id2)																//收敛在S极，对角度进行补偿
		{
			HFI.theta_Init = PLL_HFI_Para.Theta + PI;
			if(HFI.theta_Init > PIX2)
				HFI.theta_Init -= PIX2;

			PLL_HFI_Para.Theta = HFI.theta_Init;
			//            LED2 = !LED2;
		}
		Sum_Id1 = 0;
		Sum_Id2 = 0;

		pi_id.Ref = 0;
		pi_id.Fbk = HFI.Idf;
		PI_Controller((M_PI_Control)&pi_id);
		//		pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
		HFI_Clock = 0;
		//        HFI_inj_volt_amp_ok = 0;
		//        hfi_cnt = 0;
	}

	//磁极极性辨识
	/*
    if(HFI_inj_volt_amp_ok == 1)
    {
    if(HFI_Init_Angle_flag == 0)
    {
        if(id_index < 10)											//判断d轴电流响应的极值
        {
            if(PARK_PCurr.Ds > id_max)
            {
                id_max = PARK_PCurr.Ds;
            }
            if(PARK_PCurr.Ds < id_min)
            {
                id_min = PARK_PCurr.Ds;
            }
            id_index ++;
        }
        else
        {
            if(id_max > (-id_min))
            {
                if(d_direction_ok_cnt1 > 100)
                {
                    HFI_Init_Angle_flag = 1;			//初始角度识别完成标志位
                    id_max = 0;
                    id_min = 0;
                    d_direction_ok_cnt1 = 0;
                    d_direction_ok_cnt2 = 0;
                    id_index = 0;
                }
                d_direction_ok_cnt1 ++;
            }
            else
            {
                if(d_direction_ok_cnt2 > 100)
                {
                    PLL_HFI_Para.Theta += PI;
                    if(PLL_HFI_Para.Theta > PIX2)
                        PLL_HFI_Para.Theta -= PIX2;
                    d_direction_ok_cnt2 = 0;
                    HFI_Init_Angle_flag = 1;
                    id_max = 0;
                    id_min = 0;
                    d_direction_ok_cnt1 = 0;
                    id_index = 0;
                }
                d_direction_ok_cnt2 ++;
            }
            id_max = 0.0f;
            id_min = 0.0f;
            id_index = 0;
        }
    }
    }
	 */


	HFI.Uin = HFI_P_Uin_offset*hfi_cos;					//d轴注入电压
	hfi_cnt	++;																//时间t

	//IPARK计算
	IPARK_PVdq.Theta = PLL_HFI_Para.Theta;
	IPARK_PVdq.Qs =  pi_id.Out;
	IPARK_PVdq.Ds = pi_id.Out + HFI.Uin;				//d轴高频方波注入
	IPARK_Cale((M_IPARK)&IPARK_PVdq);
}


//HFI速度闭环,高频方波注入
void HFI_Speed_Closeloop(void)
{
	float ISpeed_Ref_EX = 0.0f;
	float outMax_V;

	Speed_Ref_GXieLv.XieLv_Grad = Speed_Ref_OD ;   		//定义速度的梯度值
	Speed_Ref_GXieLv.Grad_Timer = Speed_Ref_Timer;    //定义速度的梯度时间
	Speed_Ref_GXieLv.XieLv_X = HFI_Speed;							//定义梯度上限，即速度环目标速度

	if(HFI_Init_Angle_flag)
	{
		Speed_Ref_GXieLv.Timer_Count	++;
		if(Speed_Ref_GXieLv.Timer_Count > Speed_Ref_GXieLv.Grad_Timer)
		{
			Speed_Ref_GXieLv.Timer_Count = 0;
			Grad_XieLv((p_GXieLv)&Speed_Ref_GXieLv);				//梯度计算
		}
		ISpeed_Ref_EX = Limit_Sat(Speed_Ref_GXieLv.XieLv_Y, HFI_Speed_Ref_Max, -HFI_Speed_Ref_Max);
	}

	//    HFI_Uin_flag =	!HFI_Uin_flag;
	HFI_Injection_Fre();

	if(HFI_Filter_flag)
	{
		//提取低频电流分量
		IIR_Butterworth(PARK_PCurr.Ds, &HFI.Idf, &D_IIR_LPF_Par);
		IIR_Butterworth(PARK_PCurr.Qs, &HFI.Iqf, &Q_IIR_LPF_Par);

	}
	else
	{
		//        //提取低频电流分量
		if(HFI_Fre_CNT ==0)
		{
			HFI.Idf = (PARK_PCurr.Ds + HFI.Id_last)*0.5f;
			HFI.Iqf = (PARK_PCurr.Qs + HFI.Iq_last)*0.5f;

			HFI.Id_last = PARK_PCurr.Ds;
			HFI.Iq_last = PARK_PCurr.Qs;
		}

	}

	//速度环
	if(Speed_Cal_time == Speed_Cal_Period)
	{
		Speed_Cal_time	%= Speed_Cal_Period;

		pi_spd.Ref = ISpeed_Ref_EX*Motor.P;
		pi_spd.Fbk = PLL_HFI_Para.Omega_F/PIX2*60;
		PI_Controller((M_PI_Control)&pi_spd);												//速度环
		//		pi_spd.OutF = pi_spd.OutF*LPF_I_RUN_B + pi_spd.Out*LPF_I_RUN_A; //环路滤波后输出
	}

	//电流环
	pi_id.Ref = 0;
	pi_id.Fbk = HFI.Idf;
	PI_Controller((M_PI_Control)&pi_id);
	//	pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;

	//防止过调制
	float maxVsMag_V = MaxVsMagPu * ADC_Sample_Filt_Para.VBUS;
	outMax_V = maxVsMag_V * maxVsMag_V -  pi_id.Out *  pi_id.Out;
	arm_sqrt_f32(outMax_V, &outMax_V);

	//Iq轴闭环
	pi_iq.Umax = 	outMax_V;
	pi_iq.Umin = -outMax_V;
	pi_iq.Ref = pi_spd.Out;
	pi_iq.Fbk = HFI.Iqf;
	PI_Controller((M_PI_Control)&pi_iq);
	//	pi_iq.OutF = pi_iq.OutF*LPF_I_RUN_B + pi_iq.Out*LPF_I_RUN_A;

	//d轴注入的高频方波电压

	if(HFI_Uin_flag)
	{
		HFI.Uin = HFI_Uin_offset;

	}
	else
		HFI.Uin = -HFI_Uin_offset;

	//IPARK计算
	IPARK_PVdq.Theta = PLL_HFI_Para.Theta;
	IPARK_PVdq.Qs =pi_iq.Out;

	IPARK_PVdq.Ds = pi_id.Out+HFI.Uin ;

	IPARK_Cale((M_IPARK)&IPARK_PVdq);
}

void HFI_P_Speed_Closeloop(void)
{
	float ISpeed_Ref_EX = 0.0f;
	float temp;
	float outMax_V;

	Speed_Ref_GXieLv.XieLv_Grad = Speed_Ref_OD ;   		//定义速度的梯度值
	Speed_Ref_GXieLv.Grad_Timer = Speed_Ref_Timer;    //定义速度的梯度时间
	Speed_Ref_GXieLv.XieLv_X = HFI_Speed;							//定义梯度上限，即速度环目标速度

	if(HFI_Init_Angle_flag)
	{
		Speed_Ref_GXieLv.Timer_Count	++;
		if(Speed_Ref_GXieLv.Timer_Count > Speed_Ref_GXieLv.Grad_Timer)
		{
			Speed_Ref_GXieLv.Timer_Count = 0;
			Grad_XieLv((p_GXieLv)&Speed_Ref_GXieLv);				//梯度计算
		}
		ISpeed_Ref_EX = Limit_Sat(Speed_Ref_GXieLv.XieLv_Y, HFI_Speed_Ref_Max, -HFI_Speed_Ref_Max);
	}

	if(hfi_cnt >= HFI_CNT_FLAG)
		hfi_cnt = 0;

	temp = hfi_cnt;

	hfi_cos = arm_cos_f32((temp/HFI_CNT_FLAG)*PIX2);
	hfi_sin = arm_sin_f32((temp/HFI_CNT_FLAG)*PIX2);

	//提取低频电流分量
	IIR_Butterworth(PARK_PCurr.Ds, &HFI.Idf, &D_IIR_LPF_Par);
	IIR_Butterworth(PARK_PCurr.Qs, &HFI.Iqf, &Q_IIR_LPF_Par);

	//提取高频电流分量
	//	IIR_Butterworth(PARK_PCurr.Qs, &HFI.Iqh, &Q_IIR_BPF_Par);
	arm_iir_f32_bpf(&PARK_PCurr.Qs, &HFI.Iqh);

	//速度环
	if(Speed_Cal_time == Speed_Cal_Period)
	{
		Speed_Cal_time	%= Speed_Cal_Period;

		pi_spd.Ref = ISpeed_Ref_EX*Motor.P;
		pi_spd.Fbk = PLL_HFI_Para.Omega_F/PIX2*60;
		PI_Controller((M_PI_Control)&pi_spd);												//速度环
		//			pi_spd.OutF = pi_spd.OutF*LPF_I_RUN_B + pi_spd.Out*LPF_I_RUN_A; //环路滤波后输出
	}

	//电流环
	pi_id.Ref = 0;
	pi_id.Fbk = HFI.Idf;
	PI_Controller((M_PI_Control)&pi_id);
	//	pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;

	//防止过调制
	float maxVsMag_V = MaxVsMagPu * ADC_Sample_Filt_Para.VBUS;
	outMax_V = maxVsMag_V * maxVsMag_V -  pi_id.OutF *  pi_id.OutF;
	arm_sqrt_f32(outMax_V, &outMax_V);

	//Iq轴闭环
	pi_iq.Umax = 	outMax_V;
	pi_iq.Umin = -outMax_V;
	pi_iq.Ref = pi_spd.Out;
	pi_iq.Fbk = HFI.Iqf;
	PI_Controller((M_PI_Control)&pi_iq);
	//	pi_iq.OutF = pi_iq.OutF*LPF_I_RUN_B + pi_iq.Out*LPF_I_RUN_A;

	HFI.Uin = HFI_P_Uin_offset*hfi_cos;
	hfi_cnt	++;

	IPARK_PVdq.Theta = PLL_HFI_Para.Theta;
	IPARK_PVdq.Qs = pi_iq.Out;
	IPARK_PVdq.Ds = pi_id.Out + HFI.Uin;
	IPARK_Cale((M_IPARK)&IPARK_PVdq);
}

//提取低频电流分量
void extract_Idq_f(void)
{
	if(HFI_Filter_flag)
	{
		//提取低频电流分量
		IIR_Butterworth(PARK_PCurr.Ds, &HFI.Idf, &D_IIR_LPF_Par);
		IIR_Butterworth(PARK_PCurr.Qs, &HFI.Iqf, &Q_IIR_LPF_Par);
	}
	else
	{
		//提取低频电流分量
		HFI.Idf = (PARK_PCurr.Ds + HFI.Id_last)*0.5f;
		HFI.Iqf = (PARK_PCurr.Qs + HFI.Iq_last)*0.5f;

		HFI.Id_last = PARK_PCurr.Ds;
		HFI.Iq_last = PARK_PCurr.Qs;
	}
}

//HFI速度环，全速度无感控制用
void HFI_Observer(float Id, float Iq)
{
	float outMax_V;
	float	angleDelta_rad;
	float angleFoc_rad;

	HFI_Uin_flag =	!HFI_Uin_flag;

	//速度环
	if(Speed_Cal_time == Speed_Cal_Period)
	{
		pi_spd.Ref = Sensorless.Speed_Ref;
		pi_spd.Fbk = Sensorless.Speed_Fbk;
		PI_Controller((M_PI_Control)&pi_spd);												//速度环
		//		pi_spd.OutF = pi_spd.OutF*LPF_I_RUN_B + pi_spd.Out*LPF_I_RUN_A; //环路滤波后输出
	}

	//电流环
	pi_id.Ref = 0;
	pi_id.Fbk = Id;
	PI_Controller((M_PI_Control)&pi_id);
	//	pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;

	//防止过调制
	float maxVsMag_V = MaxVsMagPu * ADC_Sample_Filt_Para.VBUS;
	outMax_V = maxVsMag_V * maxVsMag_V -  pi_id.Out *  pi_id.Out;
	arm_sqrt_f32(outMax_V, &outMax_V);

	//Iq轴闭环
	pi_iq.Umax = 	outMax_V;
	pi_iq.Umin = -outMax_V;
	pi_iq.Ref = pi_spd.Out;
	pi_iq.Fbk = Iq;
	PI_Controller((M_PI_Control)&pi_iq);
	//	pi_iq.OutF = pi_iq.OutF*LPF_I_RUN_B + pi_iq.Out*LPF_I_RUN_A;

	//d轴注入的高频方波电压
	if(HFI_Uin_flag)
		HFI.Uin = HFI_Uin_offset;
	else
		HFI.Uin = -HFI_Uin_offset;

	//用延时补偿计算角度
	angleDelta_rad = TS*PLL_SMO_Para.Omega_F;
	angleFoc_rad = MATH_incrAngle(Sensorless.theta, angleDelta_rad);

	//	IPARK_SMO_PVdq.Theta = Sensorless.theta;
	IPARK_SMO_PVdq.Theta = angleFoc_rad;
	IPARK_SMO_PVdq.Ds = pi_id.Out;
	IPARK_SMO_PVdq.Qs = pi_iq.Out;
	IPARK_Cale((M_IPARK)&IPARK_SMO_PVdq);

	//IPARK计算
	IPARK_PVdq.Theta = Sensorless.theta;
	//	IPARK_PVdq.Theta = angleFoc_rad;
	IPARK_PVdq.Qs = pi_iq.Out;
	IPARK_PVdq.Ds = pi_id.Out + HFI.Uin;
	IPARK_Cale((M_IPARK)&IPARK_PVdq);
}

//高频脉振方波输入
void HFI_Injection(void)
{
	float IF_Freq_EX = 0.0f;

	HFI_Uin_flag =! HFI_Uin_flag;
	//提取低频电流分量
	HFI.Idf = (PARK_PCurr.Ds + HFI.Id_last)*0.5f;
	HFI.Iqf = (PARK_PCurr.Qs + HFI.Iq_last)*0.5f;

	HFI.Id_last = PARK_PCurr.Ds;
	HFI.Iq_last = PARK_PCurr.Qs;


	IF_Freq_GXieLv.XieLv_Grad = IF_F_Grad_0D1HZ ;   //定义频率的梯度值
	IF_Freq_GXieLv.Grad_Timer = IF_F_Grad_Timer;    //定义频率的梯度时间
	IF_Freq_GXieLv.XieLv_X = IF_Fre_Max;								//定义梯度上限

	IF_Freq_GXieLv.Timer_Count ++;
	if(IF_Freq_GXieLv.Timer_Count > IF_Freq_GXieLv.Grad_Timer)
	{
		IF_Freq_GXieLv.Timer_Count = 0;
		Grad_XieLv((p_GXieLv)&IF_Freq_GXieLv);				//梯度计算
	}

	IF_Freq_EX = Limit_Sat(IF_Freq_GXieLv.XieLv_Y, Motor_Freq_Max, Motor_Freq_Min );
	IF_Theta += (float)(PIX2 * IF_Freq_EX) / TIM1_Frq;
	if(IF_Theta >= PIX2)
		IF_Theta -= PIX2;

	//速度环
	//	pi_spd.Ref = 200* Motor.P;
	//	pi_spd.Fbk = Motor.speed_E_rpm;
	//	PI_Controller((M_PI_Control)&pi_spd);												//速度环
	//	pi_spd.OutF = pi_spd.OutF*LPF_I_RUN_B + pi_spd.Out*LPF_I_RUN_A; //环路滤波后输出

	//电流环
	pi_id.Ref = 0;
	pi_id.Fbk = HFI.Idf;
	PI_Controller((M_PI_Control)&pi_id);
	//	pi_id.OutF = pi_id.OutF*LPF_HFI_B + pi_id.Out*LPF_HFI_A;
	//Iq轴闭环
	pi_iq.Ref = 2.0f;
	pi_iq.Fbk = HFI.Iqf;
	PI_Controller((M_PI_Control)&pi_iq);
	//	pi_iq.OutF = pi_iq.OutF*LPF_I_RUN_B + pi_iq.Out*LPF_I_RUN_A;

	if(HFI_Uin_flag)
		HFI.Uin = HFI_Uin_offset;
	else
		HFI.Uin = -HFI_Uin_offset;

	//IPARK计算
	//	IPARK_PVdq.Theta = Motor.E_theta;
	IPARK_PVdq.Theta = IF_Theta;
	IPARK_PVdq.Qs = pi_iq.Out;
	IPARK_PVdq.Ds = pi_id.Out + HFI.Uin;
	IPARK_Cale((M_IPARK)&IPARK_PVdq);
}

