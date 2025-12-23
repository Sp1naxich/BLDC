#include "FOC_Control.h"

/*
**Clarkb变换
**Ia+Ib+Ic=0
**等幅值变换
**Alpha=Iu
**Beta=sqrt(3)/3*Iu+x*sqrt(3)/3*Iv
*/

void CLARKE_Cale(M_CLARKE pv)
{
    pv->Alpha =-(pv->Iu + pv->Iv) ;
    pv->Beta =  0.5773502691896f*(pv->Iu - pv->Iv);
}


/*
**Park变换
*/
void PARK_Cale(M_PARK pv)
{
    pv->Ds = arm_cos_f32(pv->Theta) * pv->Alpha + arm_sin_f32(pv->Theta) * pv->Beta;
    pv->Qs = arm_cos_f32(pv->Theta) * pv->Beta - arm_sin_f32(pv->Theta) * pv->Alpha;
}


/*
**IPark变换
*/
void IPARK_Cale(M_IPARK pv)
{
    pv->Alpha = arm_cos_f32(pv->Theta) * pv->Ds - arm_sin_f32(pv->Theta) * pv->Qs;
    pv->Beta = arm_sin_f32(pv->Theta) * pv->Ds + arm_cos_f32(pv->Theta) * pv->Qs;
}

