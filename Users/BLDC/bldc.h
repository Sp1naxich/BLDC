/*
 * bldc.h
 *
 *  Created on: 2025年12月10日
 *      Author: X
 */

#ifndef BLDC_BLDC_H_
#define BLDC_BLDC_H_

#include "user.h"

#define BLDC_TIM_PWM_PERIOD 8399

enum BLDC_CONTROL_CMD{
	BLDC_NONE=0,
	BLDC_RUN,
	BLDC_STOP=5
};

enum BLDC_CHANGE_STEP{
	BLDC_STEP_UV=1,
	BLDC_STEP_UW,
	BLDC_STEP_VW,
	BLDC_STEP_VU,
	BLDC_STEP_WU,
	BLDC_STEP_WV
};

typedef struct{
	enum BLDC_CONTROL_CMD Ctrl;
	float duty;
}BLDC_Control;

typedef struct{
	uint8_t step;
	enum BLDC_CHANGE_STEP CurtStep;
	enum BLDC_CHANGE_STEP NextStep;
}BLDC_Status;

void BLDC_Run(void);
void BLDC_Stop(void);
void BLDC_StepChange(void);

void m1_uhvl(void);
void m1_uhwl(void);
void m1_vhwl(void);
void m1_vhul(void);
void m1_whul(void);
void m1_whvl(void);


#endif /* BLDC_BLDC_H_ */
