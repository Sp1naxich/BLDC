#ifndef __FILTER_H
#define __FILTER_H

#include "delay.h"
#include "stdint.h"
#include "arm_math.h"


#define NumStages  									2      						/* 2阶IIR滤波的个数 */
#define BLOCK_SIZE           				1     		  			/* 调用一次arm_biquad_cascade_df1_f32处理的采样点个数 */
#define ScaleValue_BPF							0.02008336556f		/*缩放系数=BPF定标值^2*/

/*
//BPF定标值:
0.141715791513194855344437428357196040452
*/


void BPF_Init(void);
void arm_iir_f32_bpf(float *inputF32, float *outputF32);

typedef struct
{
    float states0;
    float states1;
    float b0;
    float b1;
    float b2;
    float a0;
    float a1;
    float a2;
    float gain0;
    float gain1;
} IIR_BUTTERWORTH_DEF;

extern IIR_BUTTERWORTH_DEF 		WE_IIR_LPF_Par;
extern IIR_BUTTERWORTH_DEF 		D_IIR_LPF_Par;
extern IIR_BUTTERWORTH_DEF 		Q_IIR_LPF_Par;
extern IIR_BUTTERWORTH_DEF 		A_IIR_LPF_Par;
extern IIR_BUTTERWORTH_DEF 		B_IIR_LPF_Par;
extern IIR_BUTTERWORTH_DEF		Q_IIR_BPF_Par;
extern IIR_BUTTERWORTH_DEF		D_IIR_BPF_Par;
extern IIR_BUTTERWORTH_DEF 		ERR_IIR_LPF_Par;
extern IIR_BUTTERWORTH_DEF		A_IIR_HPF_Par;
extern IIR_BUTTERWORTH_DEF		B_IIR_HPF_Par;
extern IIR_BUTTERWORTH_DEF		D_IIR_HPF_Par;

extern float WE_IIR_LPF_Coeff[8];
extern float D_IIR_LPF_Coeff[8];
extern float Q_IIR_LPF_Coeff[8];
extern float A_IIR_LPF_Coeff[8];
extern float B_IIR_LPF_Coeff[8];
extern float Q_IIR_BPF_Coeff[8];
extern float D_IIR_BPF_Coeff[8];
extern float ERR_IIR_LPF_Coeff[8];
extern float A_IIR_HPF_Coeff[8];
extern float B_IIR_HPF_Coeff[8];
extern float D_IIR_HPF_Coeff[8];

void IIR_Butterworth_Coefficient_Init(float temp[8],IIR_BUTTERWORTH_DEF* iir_butterworth_temp);
void IIR_Butterworth(float in,float* out,IIR_BUTTERWORTH_DEF* iir_butterworth_temp);

#endif

