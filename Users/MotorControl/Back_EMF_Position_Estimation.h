#ifndef __BACK_EMF_POSITION_ESTIMATION
#define __BACK_EMF_POSITION_ESTIMATION

typedef struct {
    float Ed;
    float Eq;
    float theta_err;
    float theta_err_last;
    float sum_pll;
    float speed_est;
    float theta_est;
    float kp_pll;
    float ki_pll;
} BEMF_PE, *p_BEMF_PE;

#define BEMF_PE_DEFAULTS {0,0,0,0,0,0,0,0,0}			//初始化参数

extern BEMF_PE			B_EMF_PE;

void BEMF_Position_Estimate(p_BEMF_PE pv);

#endif

