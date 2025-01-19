#include "stm32f10x.h"                  // Device header

#include "Delay.h"
#include "OLED.h"
#include "CONFgeneral.h"
#include "comCONFgeneral.h"
#include "hwCONFIGgen.h"
#include "drive.h"

#include "map.h"

//end of test area

void Setup(void){
	

	
	reuse_init();
	OLED_Init();
	drive_init();
	
	TIM_SetCompare3(TIM4,800);
	TIM_SetCompare4(TIM4,0);

	TIMER_setMODE(TIM2,1000,36,TIM_CounterMode_Up);

}

int diff=-50;
int main(void)
{
	Setup();

	while (1)
	{
		diff=diff>50?-50:diff+1;
		drive_DIFFsetPWM34(diff);
		OLED_ShowString(1,1,"++");
		OLED_ShowSignedNum(2,1,diff,5);
  Delay_ms(100);
		OLED_ShowString(1,1,"+ ");
		OLED_ShowNum(1,3,TIM4->ARR,5);

				OLED_ShowSignedNum(3,5,TIM4->CCR3,5);
		Delay_ms(100);
	}
}




