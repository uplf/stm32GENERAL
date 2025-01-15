#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CONFgeneral.h"
#include "hwCONFIGgen.h"

#include "map.h"

uint16_t duty=0;
//end of test area

void Setup(void){

	OLED_Init();
	PIN_setMODE(GPIOA,GPIO_Pin_2,GPIO_Mode_AIN);
	sADC_setMODE(ADC_Channel_2,ADC1);
	sADC_caliSTART(ADC1);
}
uint16_t ADValue;			//定义AD值变量
float Voltage;				//定义电压变量
int main(void)
{
	Setup();

	while (1)
	{
		
		ADValue = sADC_getDATA(ADC1);					//获取AD转换的值
		Voltage = (float)ADValue / 4095 * 3.3;		//将AD值线性变换到0~3.3的范围，表示电压
		
		OLED_ShowNum(1, 9, ADValue, 4);				//显示AD值
		OLED_ShowNum(2, 9, Voltage, 1);				//显示电压值的整数部分
		OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2);	//显示电压值的小数部分

				Delay_ms(500);
	}
}
