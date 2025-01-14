#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CONFgeneral.h"
#include "hwCONFIGgen.h"




//end of test area

void Setup(void){
	OLED_Init();
	PIN_setMODE(GPIOA,GPIO_Pin_10|GPIO_Pin_9,_OUTPUT);
	PIN_setMODE(GPIOB,GPIO_Pin_1,GPIO_Mode_IPU);

	
}

int main(void)
{
	Setup();

	while (1)
	{
		if(PIN_readBIT(GPIOB,GPIO_Pin_1))PIN_writeBIT(GPIOA,GPIO_Pin_10,_HIGH);
		else PIN_writeBIT(GPIOA,GPIO_Pin_10,_LOW);
		if(PIN_readBIT(GPIOA,GPIO_Pin_10))PIN_writeBIT(GPIOA,GPIO_Pin_9,_LOW);
		else PIN_writeBIT(GPIOA,GPIO_Pin_9,_HIGH);

		Delay_ms(50);
		
	}
}
