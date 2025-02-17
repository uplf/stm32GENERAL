#include "stm32f10x.h"                  // Device header
#include "LED.h"

/**
  * 函    数：LED初始化
  * 参    数：无
  * 返 回 值：无
  */
void LED_Init(void)
{
	PIN_setMODE(GPIOC,GPIO_Pin_13|GPIO_Pin_14,_OUTPUT);
	PIN_writeBIT(GPIOC,GPIO_Pin_13|GPIO_Pin_14,_LOW);
}

void LED_Blink(void){
	TIM_Cmd(TIM3,DISABLE);
	PIN_writeBIT(GPIOC,GPIO_Pin_13,_HIGH);
	Delay_ms(1000);
	PIN_writeBIT(GPIOC,GPIO_Pin_13,_LOW);
	TIM_Cmd(TIM3,ENABLE);
}
