#include "stm32f10x.h"
#include "CONFgeneral.h"
#include "detCalc.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "grey.h"
#include "drive.h"
#include "Interrupt.h"

//Ñ²ÏßÖÐ¶Ï

void intInit(){
	EXTI_setGROUP(NVIC_PriorityGroup_2);
	freqTIMER_setMODE(TIM1,5,TIM_CounterMode_Up);
	INT_setNVIC(timerINT_preset(TIM2),1,1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		wytest++;
		//½Ç¶ÈPID
		if(MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw)==0)
		{
			AngleLeft.current=Yaw;
			AngleRight.current=Yaw;
			DrivePidCalc(&AngleRight,&AngleLeft,&SpeedR,&SpeedL);
			drive_setPWM34(SpeedR,SpeedL);
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}