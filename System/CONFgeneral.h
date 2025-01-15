#ifndef __GPIO_CONF_GEN
#define __GPIO_CONF_GEN
#include "stm32f10x.h"                  // Device header


/*****section1:GPIO_configeration***

*/
#define _INPUT GPIO_Mode_IN_FLOATING
#define _OUTPUT GPIO_Mode_Out_PP
#define _HIGH Bit_SET
#define _LOW Bit_RESET


void PIN_setMODE(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,GPIOMode_TypeDef MODE);
//set the mode of a pin
/*
param3 MODE: 
	GPIO_MODE_IN/OUT_xxx - 
	_INPUT->float浮动输入
	_OUTPUT->PP推挽输出

*/

uint8_t PIN_readBIT(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
//read the bit of pin except for analog mode
void PIN_writeBIT(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x,BitAction);
//set the bit param3 _HIGH/_LOW


/***Section2:interrupt_configeration
*/

//step1:(only once each project|*)confirm priority group(refer the following group)
void EXTI_setGROUP(uint32_t NVIC_PriorityGroup_x);

//step2:configurate EXTI and obtain IRQn
int8_t gpioINT_setEXTI(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x,EXTITrigger_TypeDef EXTI_Trigger_x);
//for GPIO INT<above>
int8_t timerINT_preset(TIM_TypeDef* TIMx);
//for TIMER INT<above>

//step3:set NVIC using channel
void INT_setNVIC(uint8_t NVIC_IRQChannel,uint8_t PP,uint8_t SP,FunctionalState x);
/**
@code  
 The table below gives the allowed values of the pre-emption priority and subpriority according
 to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority                       
  ============================================================================================================================
@endcode
*/

//step4: INT function
/*
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)		//判断是否是外部中断14号线触发的中断
	{

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
		{
			-------
		}
		EXTI_ClearITPendingBit(EXTI_Line14);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}
*/

/* 定时器中断函数，可以复制到使用它的地方
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/


/*****Section3:timer_configuration
*/

//step1: timer_config
	void TIMER_setMODE(TIM_TypeDef* TIMx,uint16_t period,uint16_t prescaler,uint16_t TIM_CounterMode_x);
		//
	void freqTIMER_setMODE(TIM_TypeDef* TIMx,uint16_t freq,uint16_t TIM_CounterMode_x);
		//attention: 1.freq refers to CK_CNT_OV(overflow) 2. ARR->10000  3.CK_PSC->72M	

//step2:apply <section2->step2> for INT, <following> for OC & IC
	//step2(PWM).1,setPWMmode
		void PWM_setMODE(TIM_TypeDef* TIMx);
	//step2(PWM).2,IO_AF
		void PWM_setIO(TIM_TypeDef* TIMx,uint16_t AF_INDEX);
		/*param2:
		the PWM output port index, which can be checked on GPIO_function_sheet, should decide
		the corresponding bit of AF_INDEX's binary
		ej:to activate PWM CH1、CH3,param2 should be 0b...0101
		*/
	//stepx(PWM).计算CCR
		uint16_t dutyPWM_calCCR(TIM_TypeDef* TIMx,uint16_t duty);
		
	//step2(IC).1,setGPIO--GPIO_Mode_IPU
	//step2(IC).2,PWMI
		void PWMI_setMODE(TIM_TypeDef* TIMx,uint16_t TIM_Channel_x);//##无效果
	//stepx(IC),read
		uint32_t PWMI_getFREQ(TIM_TypeDef* TIMx);//##无效果
		uint32_t PWMI_getDUTY(TIM_TypeDef* TIMx);//##无效果
//step3:manipulate timer with TIM_Cmd(TIMx,EN/DISABLE)<omit>




/*******Section4:AD & DMA
*/
//AD step1:GPIO<check sheet>_CONF->CPIO_Mode_AIN
//AD step2:ADC_MODE
	void sADC_setMODE(uint8_t ADC_Channel_x,ADC_TypeDef* ADCx);
//AD step3:calibration and start
	void sADC_caliSTART(ADC_TypeDef* ADCx);//校准启动
//AD stepx:read
	uint16_t sADC_getDATA(ADC_TypeDef* ADCx);
//对于不是单通道的，需要自己配置，略作修改即可


//DMA这里省略



/******Section5:Communication
1.usart
2.iic
3.spi
*/
	//part1:serial
		//step1:config pins pinA->AF_PP,pinB->IPU
		void USART_setMODE()
#endif
