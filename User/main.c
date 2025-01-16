#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CONFgeneral.h"
#include "comCONFgeneral.h"
#include "hwCONFIGgen.h"

#include "map.h"

//end of test area

void Setup(void){

	OLED_Init();
I2C_GenerateSTART(I2C2, ENABLE);		
	
}


int main(void)
{
	Setup();

	while (1)
	{
		OLED_ShowString(1,1,"++");
  Delay_ms(100);
		OLED_ShowString(1,1,"+ ");

		Delay_ms(100);
	}
}




