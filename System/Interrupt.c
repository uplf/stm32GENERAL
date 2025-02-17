#include "stm32f10x.h"
#include "CONFgeneral.h"
#include "detCalc.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "grey.h"
#include "drive.h"
#include "Interrupt.h"


//巡线中断

void intInit(){
	EXTI_setGROUP(NVIC_PriorityGroup_2);
	freqTIMER_setMODE(TIM3,5,TIM_CounterMode_Up);
	INT_setNVIC(timerINT_preset(TIM3),1,1,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler(void)
{
	
	//MPU6050 scan->mpuINDEX
	//grey scan->greyINDEX
	//runMode->1 ;->2 ;->0
	
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);//检测MPU6050并完成角度解算
		GREY_init();
		GREY_ISINDEX=grey_scancal(&GREY_INDEX);
		grey_recal(&GREY_INDEX);
		PIN_writeBIT(GPIOC,GPIO_Pin_13,GREY_ISINDEX&0x01);
		PIN_writeBIT(GPIOC,GPIO_Pin_14,GREY_ISINDEX&0x02);
		
		GreyLeft.current=GREY_INDEX;
		GreyRight.current=GREY_INDEX;
		AngleLeft.current=Yaw;
		AngleRight.current=Yaw;
		
		//角度PID
		
		switch(RunMode){
			case 0:{
				drive_setPWM34(0,0);
				break;
			}
			case 1:{
				DrivePidCalc(&GreyRight,&GreyLeft,&SpeedR,&SpeedL);
				drive_setPWM34(SpeedR,SpeedL);
				break;
			}
			case 2:{
				DrivePidCalc(&AngleRight,&AngleLeft,&SpeedR,&SpeedL);
				drive_setPWM34(SpeedR,SpeedL);
			}
		}
		//控制

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}