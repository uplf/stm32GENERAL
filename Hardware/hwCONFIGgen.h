#ifndef __HW_CONF_GENERAL_H
#define __HW_CONF_GENERAL_H

#include "stm32f10x.h"                  // Device header
#include "userSetup.h"
#include "CONFgeneral.h"
#include "Delay.h"
#include "comCONFgeneral.h"


//IIC source
void I2C_writeREG(IIC_group g,uint8_t slaveADDR, uint8_t regADDR,uint8_t data);//未测试
uint8_t readREG(IIC_group g,uint8_t slaveADDR,uint8_t regADDR);//未测试

//串口调试方法
void USARTdebug_init();



#endif
