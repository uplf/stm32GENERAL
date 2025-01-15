#ifndef __MAP
#define __MAP
#include "stm32f10x.h"                  // Device header

uint32_t GPIOToRCCPeriph(GPIO_TypeDef*);//映射函数，将GPIOx转为RCC配置变量
uint8_t GPIOPinToPinSource(uint16_t GPIO_Pin);
uint8_t GPIOToPortSource(GPIO_TypeDef *GPIOx);
uint32_t GPIOPinToEXTILine(uint16_t GPIO_Pin);
int8_t EXTILineToIRQn(uint32_t EXTI_Line);
IRQn_Type TIMxtoIRQn(TIM_TypeDef *TIMx);
uint16_t TimIndexToPWMPins(TIM_TypeDef *TIMx,uint16_t AF_INDEX);
uint32_t TIMxtoRCCPeriph(TIM_TypeDef *TIMx);
uint32_t ADCxtoRCC_Periph(ADC_TypeDef *ADCx);
#endif
