#include "drive.h"
#include "OLED.h"


void drive_init(){
	//PWMÊä³ö
	PWM_setIO(TIM4,0x04|0x08);
	TIMER_setMODE(TIM4,100,36,TIM_CounterMode_Up);
	PWM_setMODE(TIM4,0x01|0x02|0x04|0x08);

	TIM_Cmd(TIM4,ENABLE);
	
	PIN_setMODE(DRIVE_AIN1_GROUP,DRIVE_AIN1_PIN,_OUTPUT);
	PIN_setMODE(DRIVE_AIN2_GROUP,DRIVE_AIN2_PIN,_OUTPUT);
	PIN_setMODE(DRIVE_BIN1_GROUP,DRIVE_BIN1_PIN,_OUTPUT);
	PIN_setMODE(DRIVE_BIN2_GROUP,DRIVE_BIN2_PIN,_OUTPUT);
	PIN_writeBIT(DRIVE_AIN1_GROUP,DRIVE_AIN1_PIN,0);
	PIN_writeBIT(DRIVE_AIN2_GROUP,DRIVE_AIN2_PIN,0);
	PIN_writeBIT(DRIVE_BIN1_GROUP,DRIVE_BIN1_PIN,0);
	PIN_writeBIT(DRIVE_BIN2_GROUP,DRIVE_BIN2_PIN,0);
}

void drive_setPWM34(int16_t duty3,int16_t duty4){
	drive_setPWM3(duty3);
	drive_setPWM4(duty4);
}
void drive_DIFFsetPWM34(int16_t DIFFduty){
	drive_setPWM34(BASIC_SPEEDR+DIFFduty,BASIC_SPEEDL-DIFFduty);
}


void drive_setPWM3(int16_t speed){

	if(speed>=0){
		drive_invPWM3(_DISABLE);
		TIM_SetCompare3(TIM4,dutyPWM_calCCR(TIM4,speed,100));
	}
	else{
		if(!INV_ABLE){
			TIM_SetCompare3(TIM4,0);
			return;
		}
		drive_invPWM3(_ENABLE);
		TIM_SetCompare3(TIM4,dutyPWM_calCCR(TIM4,-speed,100));
	}
}
void drive_setPWM4(int16_t speed){
	if(speed>=0){
		drive_invPWM4(_DISABLE);
		TIM_SetCompare4(TIM4,dutyPWM_calCCR(TIM4,speed,100));
	}
	else{
		if(!INV_ABLE){
			TIM_SetCompare4(TIM4,0);
			return;
		}
		drive_invPWM4(_ENABLE);
		TIM_SetCompare4(TIM4,dutyPWM_calCCR(TIM4,-speed,100));
	}
}


void drive_invPWM3(int8_t alter){
 if(alter==-1)
 {
	 	PIN_writeBIT(DRIVE_AIN1_GROUP,DRIVE_AIN1_PIN,0);
	 	PIN_writeBIT(DRIVE_AIN2_GROUP,DRIVE_AIN2_PIN,0);
	  return;
 }
 else{
	 	PIN_writeBIT(DRIVE_AIN1_GROUP,DRIVE_AIN1_PIN,alter);
	 	PIN_writeBIT(DRIVE_AIN2_GROUP,DRIVE_AIN2_PIN,!alter);
 }
}

void drive_invPWM4(int8_t alter){
 if(alter==-1)
 {
	 	PIN_writeBIT(DRIVE_BIN1_GROUP,DRIVE_BIN1_PIN,0);
	 	PIN_writeBIT(DRIVE_BIN2_GROUP,DRIVE_BIN2_PIN,0);
	  return;
 }
 else{
	 	PIN_writeBIT(DRIVE_AIN1_GROUP,DRIVE_BIN1_PIN,alter);
	 	PIN_writeBIT(DRIVE_BIN2_GROUP,DRIVE_BIN2_PIN,!alter);
 }
}
