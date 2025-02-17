#ifndef __INTERRUPT_H
#define __INTERRUPT_H
#include "stm32f10x.h"                  // Device header
#include "userSetup.h"



void intInit();
void TIM1_IRQHandler(void);





#endif