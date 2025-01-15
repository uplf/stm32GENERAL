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
uint16_t ADValue;			//����ADֵ����
float Voltage;				//�����ѹ����
int main(void)
{
	Setup();

	while (1)
	{
		
		ADValue = sADC_getDATA(ADC1);					//��ȡADת����ֵ
		Voltage = (float)ADValue / 4095 * 3.3;		//��ADֵ���Ա任��0~3.3�ķ�Χ����ʾ��ѹ
		
		OLED_ShowNum(1, 9, ADValue, 4);				//��ʾADֵ
		OLED_ShowNum(2, 9, Voltage, 1);				//��ʾ��ѹֵ����������
		OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2);	//��ʾ��ѹֵ��С������

				Delay_ms(500);
	}
}
