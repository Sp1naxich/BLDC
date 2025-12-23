/*
 * bldc.c
 *
 *  Created on: 2025年12月10日
 *      Author: X
 */
#include "bldc.h"
#include "user.h"
#include "sys.h"

extern TIM_HandleTypeDef htim1;

BLDC_Control bldcControl={BLDC_NONE,0.95f};
BLDC_Status bldcStatus={0};
uint8_t oldStep=0;

/* 上下桥臂的导通情况，共6种，也称为6步换向 */
/* 正点原子真值表顺序==>>顺序:6,2,3,1,5,4 */

void motor_UHVL(void) {
	PIN_UH_OUT=1;
	PIN_VH_OUT=0;
	PIN_WH_OUT=0;
	PIN_UL_OUT=0;
	PIN_VL_OUT=1;
	PIN_WL_OUT=0;
}

void motor_UHWL(void) {
	PIN_UH_OUT=1;
	PIN_VH_OUT=0;
	PIN_WH_OUT=0;
	PIN_UL_OUT=0;
	PIN_VL_OUT=0;
	PIN_WL_OUT=1;
}

void motor_VHWL(void) {
	PIN_UH_OUT=0;
	PIN_VH_OUT=1;
	PIN_WH_OUT=0;
	PIN_UL_OUT=0;
	PIN_VL_OUT=0;
	PIN_WL_OUT=1;
}

void motor_VHUL(void) {
	PIN_UH_OUT=0;
	PIN_VH_OUT=1;
	PIN_WH_OUT=0;
	PIN_UL_OUT=1;
	PIN_VL_OUT=0;
	PIN_WL_OUT=0;
}

void motor_WHUL(void) {
	PIN_UH_OUT=0;
	PIN_VH_OUT=0;
	PIN_WH_OUT=1;
	PIN_UL_OUT=1;
	PIN_VL_OUT=0;
	PIN_WL_OUT=0;
}

void motor_WHVL(void) {
	PIN_UH_OUT=0;
	PIN_VH_OUT=0;
	PIN_WH_OUT=1;
	PIN_UL_OUT=0;
	PIN_VL_OUT=1;
	PIN_WL_OUT=0;
}


//AB_AC_BC_BA_CA_CB
//UV_UW_VW_VU_WU_WV
//00_01_02_03_04_05
void (*motorDrv[6])(void) = {motor_UHVL,motor_UHWL,motor_VHWL,motor_VHUL,motor_WHUL,motor_WHVL};

void BLDC_Run(void){
	bldcControl.Ctrl=BLDC_RUN;
}

void BLDC_SoftDeadAera(void){
	PIN_UH_OUT=0;
	PIN_VH_OUT=0;
	PIN_WH_OUT=0;
	PIN_UL_OUT=0;
	PIN_VL_OUT=0;
	PIN_WL_OUT=0;
}

void BLDC_StepChange(void){
	if (bldcControl.Ctrl==BLDC_RUN){
		switch (bldcStatus.step){
		case 1:
			if (oldStep!=1){
				BLDC_SoftDeadAera();
				motorDrv[0]();
				oldStep=1;
			}
			break;
		case 2:
			if (oldStep!=2){
				BLDC_SoftDeadAera();
				motorDrv[1]();
				oldStep=2;
			}
			break;
		case 3:
			if (oldStep!=3){
				BLDC_SoftDeadAera();
				motorDrv[2]();
				oldStep=3;
			}
			break;
		case 4:
			if (oldStep!=4){
				BLDC_SoftDeadAera();
				motorDrv[3]();
				oldStep=4;
			}
			break;
		case 5:
			if (oldStep!=5){
				BLDC_SoftDeadAera();
				motorDrv[4]();
				oldStep=5;
			}
			break;
		case 6:
			if (oldStep!=6){
				BLDC_SoftDeadAera();
				motorDrv[5]();
				oldStep=6;
			}
			break;
		}
	}else if(bldcControl.Ctrl==BLDC_STOP){
		BLDC_SoftDeadAera();
	}
}

void BLDC_Stop(void){
	bldcControl.Ctrl=BLDC_STOP;
}
