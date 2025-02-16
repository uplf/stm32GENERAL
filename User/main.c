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

//end of test area

//TIP****
int status=0;
uint8_t x[]={0x1,0x2,0x3,0x4};
//mpu6050的相关参数和操作
float Axo,Ayo,Azo;	//各个姿态角的初始值
float Axc,Ayc,Azc;		//各个姿态角的检测值
float Axt,Ayt,Azt;	//各个姿态角的目标值

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
	//W25Q64_Init();

}


float Pitch,Roll,Yaw,TargetAngle;								//俯仰角默认跟中值一样，翻滚角，偏航角
short ax,ay,az,gx,gy,gz;							//加速度，陀螺仪角速度


int main(void)
{

	Setup();
	
	
	while(!Key_GetNum())
	{
		MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);
		MPU_Get_Gyroscope(&gx,&gy,&gz);
		MPU_Get_Accelerometer(&ax,&ay,&az);
		OLED_ShowSignedNum(1,1,Yaw,5);
		Delay_ms(5);
	}
	AngleLeft.target=Yaw;
	AngleRight.target=Yaw;
	
	while (1)
	{
		/*
		if(grey_scancal(&GREY_INDEX)!=2){
			grey_recal(&GREY_INDEX);
			GreyRight.current=GREY_INDEX;
			GreyLeft.current=GREY_INDEX;
			DrivePidCalc(&GreyRight,&GreyLeft,&SpeedR,&SpeedL);
			drive_setPWM34(SpeedR,SpeedL);
		}
		*/
		if(MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw)==0)
		{
			AngleLeft.current=Yaw;
			AngleRight.current=Yaw;
			DrivePidCalc(&AngleRight,&AngleLeft,&SpeedR,&SpeedL);
			drive_setPWM34(SpeedR,SpeedL);
		}

		OLED_ShowBinNum(2,1,Read_GREY(),8);
		Delay_ms(5);
	}

}


