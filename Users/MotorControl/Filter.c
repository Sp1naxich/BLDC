#include "Filter.h"

IIR_BUTTERWORTH_DEF 		WE_IIR_LPF_Par;
IIR_BUTTERWORTH_DEF 		D_IIR_LPF_Par;
IIR_BUTTERWORTH_DEF 		Q_IIR_LPF_Par;
IIR_BUTTERWORTH_DEF 		A_IIR_LPF_Par;
IIR_BUTTERWORTH_DEF 		B_IIR_LPF_Par;
IIR_BUTTERWORTH_DEF		  Q_IIR_BPF_Par;
IIR_BUTTERWORTH_DEF		  D_IIR_BPF_Par;
IIR_BUTTERWORTH_DEF 		ERR_IIR_LPF_Par;
IIR_BUTTERWORTH_DEF			A_IIR_HPF_Par;
IIR_BUTTERWORTH_DEF			B_IIR_HPF_Par;
IIR_BUTTERWORTH_DEF			D_IIR_HPF_Par;

//LPF Fc=50Hz; BPF Fc1=500Hz,Fc2=1500Hz; HPF Fc=8000Hz;
float WE_IIR_LPF_Coeff[8]  = {1.0f,  1.0f,  0.0f, 1.0f, -0.981325890492688258f, 0.0f, 0.0093370547536558f, 1.0f};			//60Hz
float D_IIR_LPF_Coeff[8]   = {1.0f,  1.0f,  0.0f, 1.0f, -0.984414127416096907f, 0.0f, 0.0077929362919516f, 1.0f};
float Q_IIR_LPF_Coeff[8]   = {1.0f,  1.0f,  0.0f, 1.0f, -0.984414127416096907f, 0.0f, 0.0077929362919516f, 1.0f};
float A_IIR_LPF_Coeff[8]   = {1.0f,  1.0f,  0.0f, 1.0f, -0.984414127416096907f, 0.0f, 0.0077929362919516f, 1.0f};
float B_IIR_LPF_Coeff[8]   = {1.0f,  1.0f,  0.0f, 1.0f, -0.984414127416096907f, 0.0f, 0.0077929362919516f, 1.0f};
float Q_IIR_BPF_Coeff[8]   = {1.0f,  0.0f, -1.0f, 1.0f, -1.532373232577131095f, 0.6128007881399321f, 0.19359960593003397f, 1.0f};
float D_IIR_BPF_Coeff[8]   = {1.0f,  0.0f, -1.0f, 1.0f, -1.532373232577131095f, 0.6128007881399321f, 0.19359960593003397f, 1.0f};
float ERR_IIR_LPF_Coeff[8] = {1.0f,  1.0f,  0.0f, 1.0f, -0.984414127416096907f, 0.0f, 0.0077929362919516f, 1.0f};
float A_IIR_HPF_Coeff[8] 	 = {1.0f, -1.0f,  0.0f, 1.0f,  0.509525449494428684f, 0.0f, 0.2452372752527856f, 1.0f};
float B_IIR_HPF_Coeff[8] 	 = {1.0f, -1.0f,  0.0f, 1.0f,  0.509525449494428684f, 0.0f, 0.2452372752527856f, 1.0f};
float D_IIR_HPF_Coeff[8] 	 = {1.0f, -1.0f,  0.0f, 1.0f,  0.509525449494428684f, 0.0f, 0.2452372752527856f, 1.0f};

u32 blockSize = BLOCK_SIZE;

static float IIRStateF32_BPF[4*NumStages];                      /* 状态缓存 */

/* 巴特沃斯带通滤波器系数500Hz 1500Hz*/
const float IIRCoeffs32BP[5*NumStages] = {
    1.0f,  0.0f,  -1.0f,     1.845151097222276215248371045163366943598f,
    -0.87461605116377438839947444648714736104f,
    1.0f,  0.0f,  -1.0f,     1.583794356962601845850713289109990000725f,
    -0.733294955202540621463924708223203197122f
};

arm_biquad_casd_df1_inst_f32 S;

//带通滤波器初始化
void BPF_Init(void)
{
    /* 初始化 */
    arm_biquad_cascade_df1_init_f32(&S, NumStages, (float *)&IIRCoeffs32BP[0], (float *)&IIRStateF32_BPF[0]);
}

/*
*********************************************************************************************************
*    函 数 名: arm_iir_f32_bpf
*    功能说明: 调用函数arm_iir_f32_bpf实现带通滤波器
*    形    参：无
*    返 回 值: 无
*********************************************************************************************************
*/

void arm_iir_f32_bpf(float *inputF32, float *outputF32)
{
    /* 实现IIR滤波 */
    arm_biquad_cascade_df1_f32(&S, inputF32,  outputF32, blockSize);

    *outputF32 = *outputF32*ScaleValue_BPF;
}

//巴特沃斯滤波器
void IIR_Butterworth_Coefficient_Init(float temp[8],IIR_BUTTERWORTH_DEF* iir_butterworth_temp)
{
    iir_butterworth_temp->b0 = temp[0];
    iir_butterworth_temp->b1 = temp[1];
    iir_butterworth_temp->b2 = temp[2];
    iir_butterworth_temp->a0 = temp[3];
    iir_butterworth_temp->a1 = temp[4];
    iir_butterworth_temp->a2 = temp[5];
    iir_butterworth_temp->gain0 = temp[6];
    iir_butterworth_temp->gain1 = temp[7];
    iir_butterworth_temp->states0 = 0.0f;
    iir_butterworth_temp->states1 = 0.0f;
}

void IIR_Butterworth(float in,float* out,IIR_BUTTERWORTH_DEF* iir_butterworth_temp)
{
    float temp;
    temp =(iir_butterworth_temp->gain0 * in
           -iir_butterworth_temp->a1 * iir_butterworth_temp->states0)
          -iir_butterworth_temp->a2 * iir_butterworth_temp->states1;
    *out = ((iir_butterworth_temp->b0 * temp + iir_butterworth_temp->b1 * iir_butterworth_temp->states0) +
            iir_butterworth_temp->b2 * iir_butterworth_temp->states1)*iir_butterworth_temp->gain1;

    iir_butterworth_temp->states1 = iir_butterworth_temp->states0;
    iir_butterworth_temp->states0 = temp;
}

