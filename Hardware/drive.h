#ifndef __DRIVE_H
#define __DRIVE_H

#include "stm32f10x.h"                  // Device header
#include "CONFgeneral.h"
#include "userSetup.h"

#define _ENABLE 1
#define _DISABLE 0

void drive_init();
void drive_setPWM3(int16_t);
void drive_setPWM4(int16_t);
//<following>不受正反转限制
void drive_invPWM3(int8_t);
void drive_invPWM4(int8_t);
//同时调速
void drive_setPWM34(int16_t,int16_t);
void drive_DIFFsetPWM34(int16_t);


#endif
