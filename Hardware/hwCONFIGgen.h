#ifndef __HW_CONF_GENERAL
#define __HW_CONF_GENERAL

#include "stm32f10x.h"                  // Device header
#include "userSetup.h"
#include "CONFgeneral.h"
#include "Delay.h"
#include "comCONFgeneral.h"


//IIC source
void I2C_writeREG(IIC_group g,uint8_t slaveADDR, uint8_t regADDR,uint8_t data);//Œ¥≤‚ ‘
uint8_t readREG(IIC_group g,uint8_t slaveADDR,uint8_t regADDR);//Œ¥≤‚ ‘



#endif
