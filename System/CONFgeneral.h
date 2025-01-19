#ifndef __GPIO_CONF_GEN
#define __GPIO_CONF_GEN
#include "stm32f10x.h"                  // Device header


/*****section1:GPIO_configeration***

*/
#define _INPUT GPIO_Mode_IN_FLOATING
#define _OUTPUT GPIO_Mode_Out_PP
#define _HIGH Bit_SET
#define _LOW Bit_RESET

void reuse_init();

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
int8_t usartINT_set(USART_TypeDef* USARTx);
//for USART INT<above>

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
/*


  * 函    数：USART1中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入

void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
	{
		uint8_t RxData = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量
		
		//使用状态机的思路，依次处理数据包的不同部分
		
		//当前状态为0，接收数据包包头
		if (RxState == 0)
		{
			if (RxData == 0xFF)			//如果数据确实是包头
			{
				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}
		//当前状态为1，接收数据包数据
		else if (RxState == 1)
		{
			Serial_RxPacket[pRxPacket] = RxData;	//将数据存入数据包数组的指定位置
			pRxPacket ++;				//数据包的位置自增
			if (pRxPacket >= 4)			//如果收够4个数据
			{
				RxState = 2;			//置下一个状态
			}
		}
		//当前状态为2，接收数据包包尾
		else if (RxState == 2)
		{
			if (RxData == 0xFE)			//如果数据确实是包尾部
			{
				RxState = 0;			//状态归0
				Serial_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除标志位
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
		void PWM_setMODE(TIM_TypeDef* TIMx,uint16_t INDEX);
	//step2(PWM).2,IO_AF
		void PWM_setIO(TIM_TypeDef* TIMx,uint16_t AF_INDEX);
		/*param2:
		the PWM output port index, which can be checked on GPIO_function_sheet, should decide
		the corresponding bit of AF_INDEX's binary
		ej:to activate PWM CH1、CH3,param2 should be 0b...0101
		*/
	//stepx(PWM).计算CCR
		uint16_t dutyPWM_calCCR(TIM_TypeDef* TIMx,uint16_t duty,uint16_t ARR);
		
	//step2(IC).1,setGPIO--GPIO_Mode_IPU
	//step2(IC).2,PWMI
		void PWMI_setMODE(TIM_TypeDef* TIMx,uint16_t TIM_Channel_x);//##无效果
	//stepx(IC),read
		uint32_t PWMI_getFREQ(TIM_TypeDef* TIMx);//##无效果
		uint32_t PWMI_getDUTY(TIM_TypeDef* TIMx);//##无效果
//step3:manipulate timer with TIM_Cmd(TIMx,EN/DISABLE)<omit>

//inner part
void TIM_OCxInit(TIM_TypeDef* , TIM_OCInitTypeDef*,uint16_t);


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

		
#endif
