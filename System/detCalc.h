#ifndef __DETCALC_H
#define __DETCALC_H
#include "stm32f10x.h"                  // Device header
#include "userSetup.h"


typedef struct{
	float P;
	float I;
	float D;
	
	float LastError;
	float PrevError;
	float IntegralError;
	
	float target;
	float current;
	int8_t circleCalc;
}PIDgroup,PIDangle;

void pidCalc(PIDgroup*,int16_t*);
void DrivePidCalc(PIDgroup*,PIDgroup*,int16_t*,int16_t*);
void PIDgroupInit();

extern PIDgroup GreyLeft;
extern PIDgroup GreyRight;

extern PIDangle AngleLeft;
extern PIDangle AngleRight;
#endif
