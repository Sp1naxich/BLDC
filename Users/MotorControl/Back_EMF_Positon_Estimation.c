#include "FOC_Control.h"
#include <Back_EMF_Position_Estimation.h>
/*根据Ed=0，进行PLL计算theta和Omega
	初步只对Id进行电流闭环，给定Vq进行调速
*/

BEMF_PE			B_EMF_PE=BEMF_PE_DEFAULTS;

float Theta_Err_Limit = 1.0471f;

void BEMF_Position_Estimate(p_BEMF_PE pv)
{
    //反电动势计算
    pv->Ed = IPARK_PVdq.Ds - Motor.Rs*PARK_PCurr.Ds + pv->speed_est*Motor.Ls*PARK_PCurr.Qs;
    pv->Eq = IPARK_PVdq.Qs - Motor.Rs*PARK_PCurr.Qs - pv->speed_est*Motor.Ls*PARK_PCurr.Ds;

    pv->theta_err = -pv->Ed / pv->Eq;

    //最大误差角度限制
    if(my_abs(pv->theta_err) > Theta_Err_Limit)
    {
        if(pv->theta_err > 0)
            pv->theta_err = Theta_Err_Limit;
        else if(pv->theta_err < 0)
            pv->theta_err = -Theta_Err_Limit;
    }

    //关于Ed=0的PLL
//	pv->sum_pll += pv->ki_pll*pv->theta_err;
    pv->speed_est += pv->kp_pll*(pv->theta_err - pv->theta_err_last) + TS*pv->ki_pll*pv->theta_err;
//	pv->speed_est = pv->sum_pll + pv->kp_pll * pv->theta_err;
    pv->theta_err_last = pv->theta_err;
    pv->theta_est += pv->speed_est*TS;

    if(pv->theta_est > PIX2)
        pv->theta_est -= PIX2;
    else if(pv->theta_est < 0)
        pv->theta_est += PIX2;
}

