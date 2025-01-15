#include "map.h"




//映射区mapping
uint32_t GPIOToRCCPeriph(GPIO_TypeDef* GPIOx) {
    if (GPIOx == GPIOA) {
        return RCC_APB2Periph_GPIOA;
    } else if (GPIOx == GPIOB) {
        return RCC_APB2Periph_GPIOB;
    } else if (GPIOx == GPIOC) {
        return RCC_APB2Periph_GPIOC;
    } else if (GPIOx == GPIOD) {
        return RCC_APB2Periph_GPIOD;
    } else if (GPIOx == GPIOE) {
        return RCC_APB2Periph_GPIOE;
    } else if (GPIOx == GPIOF) {
        return RCC_APB2Periph_GPIOF;
    } else if (GPIOx == GPIOG) {
        return RCC_APB2Periph_GPIOG;
    } else {
        return 0; // 无效输入
    }
}

uint8_t GPIOToPortSource(GPIO_TypeDef *GPIOx) {
    if (GPIOx == GPIOA) {
        return GPIO_PortSourceGPIOA;
    } else if (GPIOx == GPIOB) {
        return GPIO_PortSourceGPIOB;
    } else if (GPIOx == GPIOC) {
        return GPIO_PortSourceGPIOC;
    } else if (GPIOx == GPIOD) {
        return GPIO_PortSourceGPIOD;
    } else if (GPIOx == GPIOE) {
        return GPIO_PortSourceGPIOE;
    } else if (GPIOx == GPIOF) {
        return GPIO_PortSourceGPIOF;
    } else if (GPIOx == GPIOG) {
        return GPIO_PortSourceGPIOG;
    } else {
        // 无效的 GPIO 端口
        return 0xFF;
    }
}

uint8_t GPIOPinToPinSource(uint16_t GPIO_Pin) {
    switch (GPIO_Pin) {
        case GPIO_Pin_0:  return GPIO_PinSource0;
        case GPIO_Pin_1:  return GPIO_PinSource1;
        case GPIO_Pin_2:  return GPIO_PinSource2;
        case GPIO_Pin_3:  return GPIO_PinSource3;
        case GPIO_Pin_4:  return GPIO_PinSource4;
        case GPIO_Pin_5:  return GPIO_PinSource5;
        case GPIO_Pin_6:  return GPIO_PinSource6;
        case GPIO_Pin_7:  return GPIO_PinSource7;
        case GPIO_Pin_8:  return GPIO_PinSource8;
        case GPIO_Pin_9:  return GPIO_PinSource9;
        case GPIO_Pin_10: return GPIO_PinSource10;
        case GPIO_Pin_11: return GPIO_PinSource11;
        case GPIO_Pin_12: return GPIO_PinSource12;
        case GPIO_Pin_13: return GPIO_PinSource13;
        case GPIO_Pin_14: return GPIO_PinSource14;
        case GPIO_Pin_15: return GPIO_PinSource15;
        default:          return 0xFF;  // 无效 GPIO_Pin
    }
}

uint32_t GPIOPinToEXTILine(uint16_t GPIO_Pin) {
    switch (GPIO_Pin) {
        case GPIO_Pin_0:  return EXTI_Line0;
        case GPIO_Pin_1:  return EXTI_Line1;
        case GPIO_Pin_2:  return EXTI_Line2;
        case GPIO_Pin_3:  return EXTI_Line3;
        case GPIO_Pin_4:  return EXTI_Line4;
        case GPIO_Pin_5:  return EXTI_Line5;
        case GPIO_Pin_6:  return EXTI_Line6;
        case GPIO_Pin_7:  return EXTI_Line7;
        case GPIO_Pin_8:  return EXTI_Line8;
        case GPIO_Pin_9:  return EXTI_Line9;
        case GPIO_Pin_10: return EXTI_Line10;
        case GPIO_Pin_11: return EXTI_Line11;
        case GPIO_Pin_12: return EXTI_Line12;
        case GPIO_Pin_13: return EXTI_Line13;
        case GPIO_Pin_14: return EXTI_Line14;
        case GPIO_Pin_15: return EXTI_Line15;
        default:          return 0xFFFFFFFF;  // 无效 GPIO_Pin
    }
}

int8_t EXTILineToIRQn(uint32_t EXTI_Line) {
    switch (EXTI_Line) {
        case EXTI_Line0:  return EXTI0_IRQn;
        case EXTI_Line1:  return EXTI1_IRQn;
        case EXTI_Line2:  return EXTI2_IRQn;
        case EXTI_Line3:  return EXTI3_IRQn;
        case EXTI_Line4:  return EXTI4_IRQn;
        case EXTI_Line5:  // EXTI5~EXTI9 共用中断
        case EXTI_Line6:
        case EXTI_Line7:
        case EXTI_Line8:
        case EXTI_Line9:  return EXTI9_5_IRQn;
        case EXTI_Line10: // EXTI10~EXTI15 共用中断
        case EXTI_Line11:
        case EXTI_Line12:
        case EXTI_Line13:
        case EXTI_Line14:
        case EXTI_Line15: return EXTI15_10_IRQn;
        default:          return -1;  // 无效 EXTI_Line
    }
}

IRQn_Type TIMxtoIRQn(TIM_TypeDef *TIMx)
{
    if (TIMx == TIM1)
        return TIM1_UP_IRQn;          // TIM1 更新中断号
    else if (TIMx == TIM2)
        return TIM2_IRQn;             // TIM2 中断号
    else if (TIMx == TIM3)
        return TIM3_IRQn;             // TIM3 中断号
    else if (TIMx == TIM4)
        return TIM4_IRQn;             // TIM4 中断号
		/*
    else if (TIMx == TIM1_BRK)        // TIM1 断裂中断
        return TIM1_BRK_IRQn;
    else if (TIMx == TIM1_TRG_COM)    // TIM1 触发和通信中断
        return TIM1_TRG_COM_IRQn;
    else if (TIMx == TIM1_CC)         // TIM1 捕获比较中断
        return TIM1_CC_IRQn;
		*///这些是chat写出来的但是会报错
    else
        return (IRQn_Type)(-1);       // 无效定时器，返回 -1
}

uint16_t TimIndexToPWMPins(TIM_TypeDef *TIMx,uint16_t AF_INDEX){
	uint16_t AFP=0x00;
	if(!(AF_INDEX&0x8000)){
		if(TIMx==TIM1){
			AFP=(AF_INDEX&0x01)?(AFP|GPIO_Pin_8):AFP;
			AFP=(AF_INDEX&0x02)?(AFP|GPIO_Pin_9):AFP;
			AFP=(AF_INDEX&0x04)?(AFP|GPIO_Pin_10):AFP;
			AFP=(AF_INDEX&0x08)?(AFP|GPIO_Pin_11):AFP;
		}
		else if(TIMx==TIM2)
		{
			AFP=(AF_INDEX&0x01)?(AFP|GPIO_Pin_0):AFP;
			AFP=(AF_INDEX&0x02)?(AFP|GPIO_Pin_1):AFP;
			AFP=(AF_INDEX&0x04)?(AFP|GPIO_Pin_2):AFP;
			AFP=(AF_INDEX&0x08)?(AFP|GPIO_Pin_3):AFP;
		}
		else if(TIMx==TIM3)
		{
			AFP=(AF_INDEX&0x01)?(AFP|GPIO_Pin_6):AFP;
			AFP=(AF_INDEX&0x02)?(AFP|GPIO_Pin_7):AFP;
		}
	}
	else{
		if(TIMx==TIM3)
		{
			AFP=(AF_INDEX&0x04)?(AFP|GPIO_Pin_0):AFP;
			AFP=(AF_INDEX&0x08)?(AFP|GPIO_Pin_1):AFP;
		}
		else if(TIMx==TIM4)
		{
			AFP=(AF_INDEX&0x01)?(AFP|GPIO_Pin_6):AFP;
			AFP=(AF_INDEX&0x02)?(AFP|GPIO_Pin_7):AFP;
			AFP=(AF_INDEX&0x04)?(AFP|GPIO_Pin_8):AFP;
			AFP=(AF_INDEX&0x08)?(AFP|GPIO_Pin_9):AFP;
		}
	
	}
	return AFP;
}
uint32_t TIMxtoRCCPeriph(TIM_TypeDef *TIMx)
{
    if (TIMx == TIM1)             // 高级定时器 TIM1，位于 APB2 总线
        return RCC_APB2Periph_TIM1;
    else if (TIMx == TIM2)        // 普通定时器 TIM2，位于 APB1 总线
        return RCC_APB1Periph_TIM2;
    else if (TIMx == TIM3)        // 普通定时器 TIM3，位于 APB1 总线
        return RCC_APB1Periph_TIM3;
    else if (TIMx == TIM4)        // 普通定时器 TIM4，位于 APB1 总线
        return RCC_APB1Periph_TIM4;
    else if (TIMx == TIM5)        // 普通定时器 TIM5，位于 APB1 总线
        return RCC_APB1Periph_TIM5;
    else if (TIMx == TIM6)        // 基本定时器 TIM6，位于 APB1 总线
        return RCC_APB1Periph_TIM6;
    else if (TIMx == TIM7)        // 基本定时器 TIM7，位于 APB1 总线
        return RCC_APB1Periph_TIM7;
    else
        return 0;                 // 无效定时器，返回 0
}


uint32_t ADCxtoRCC_Periph(ADC_TypeDef *ADCx)
{
    if (ADCx == ADC1)            // ADC1 位于 APB2 总线
        return RCC_APB2Periph_ADC1;
    else if (ADCx == ADC2)       // ADC2 位于 APB2 总线
        return RCC_APB2Periph_ADC2;
    else
        return 0;                // 无效 ADC 外设，返回 0
}

