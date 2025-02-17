#include "stm32f10x.h"                  // Device header

#include "Delay.h"
#include "OLED.h"
#include "CONFgeneral.h"
#include "comCONFgeneral.h"
#include "hwCONFIGgen.h"
#include "drive.h"
#include "key.h"
#include "map.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "grey.h"
#include "userSetup.h"
#include "W25Q64.h"
#include "detCalc.h"
#include "Interrupt.h"
#include "LED.h"

//end of test area


void GreyTillEmety();
void AngleTillGrey();
void resetStartAngle();

int8_t wytest=0;

void Setup(void){
	//Encoder_Init();
	//USARTdebug_init();
	Key_Init();
	reuse_init();
	OLED_Init();
	
	drive_init();
	MPU6050_Init();		//MPU6050初始化
	MPU6050_DMP_Init();
	GREY_init();
	PIDgroupInit();
	intInit();
	LED_Init();
	//W25Q64_Init();

}


float Pitch,Roll,Yaw;								//俯仰角默认跟中值一样，翻滚角，偏航角
float StartAngle;
int8_t RunMode;

int main(void)
{

		Setup();
	TIM_Cmd(TIM3,DISABLE);
	resetStartAngle();
	
	drive_DIFFsetPWM34(0);
	while(1){
	TIM_Cmd(TIM3,ENABLE);
	//直行
	for(int i=1;i<=4;i++){
		AngleLeft.target=StartAngle;
		AngleRight.target=StartAngle;
		AngleTillGrey();
		GreyTillEmety();
		AngleLeft.target=StartAngle+180;
		AngleRight.target=StartAngle+180;
		CirCalF(&AngleLeft.target);
		AngleTillGrey();
		GreyTillEmety();
	}
	for(int i=0;i<=4;i++){
		
		
	}
	/*

	}
	*/
}
	
}
void GreyTillEmety(){
	RunMode=1;
	PIN_writeBIT(GPIOC,GPIO_Pin_13,_LOW);
		while (GREY_ISINDEX!=3){}
		RunMode=0;
		drive_setPWM34(0,0);
		//LED_Blink();
}
void AngleTillGrey(){
	RunMode=2;
	PIN_writeBIT(GPIOC,GPIO_Pin_13,_HIGH);
		while (GREY_ISINDEX){}
		RunMode=0;
		drive_setPWM34(0,0);
		//LED_Blink();
}
void resetStartAngle(){
		TIM_Cmd(TIM3,DISABLE);

	//角度PID

	while(!Key_GetNum())
	{
		Delay_ms(5);
		MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);
		OLED_ShowSignedNum(1,1,Yaw,5);
		
	}
	StartAngle=Yaw;
	OLED_ShowChar(1,7,'+');
	MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);
	OLED_ShowSignedNum(1,1,Yaw,5);
	OLED_ShowSignedNum(2,1,StartAngle,5);
	
	TIM_Cmd(TIM3,ENABLE);
	
}
