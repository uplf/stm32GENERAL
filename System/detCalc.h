#ifndef __DETCALC_H
#define __DETCALC_H
#include "stm32f10x.h"                  // Device header
#include "userSetup.h"


typedef struct{
	float P;
	float I;
	float D;
	
	int16_t LastError;
	int16_t PrevError;
	int16_t IntegralError;
	
	int16_t target;
	int16_t current;
	int8_t circleCalc;
}PIDgroup;

void pidCalc(PIDgroup*,int16_t*);
void DrivePidCalc(PIDgroup*,PIDgroup*,int16_t*,int16_t*);

#endif
