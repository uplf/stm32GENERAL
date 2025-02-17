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

//end of test area

//TIP****
int status=0;
uint8_t x[]={0x1,0x2,0x3,0x4};
//mpu6050����ز����Ͳ���
float Axo,Ayo,Azo;	//������̬�ǵĳ�ʼֵ
float Axc,Ayc,Azc;		//������̬�ǵļ��ֵ
float Axt,Ayt,Azt;	//������̬�ǵ�Ŀ��ֵ

int8_t wytest=0;

void Setup(void){
	//Encoder_Init();
	//USARTdebug_init();
	Key_Init();
	reuse_init();
	OLED_Init();
	drive_init();
	MPU6050_Init();		//MPU6050��ʼ��
	MPU6050_DMP_Init();
	GREY_init();
	PIDgroupInit();
	intInit();
	//W25Q64_Init();

}


float Pitch,Roll,Yaw,TargetAngle;								//������Ĭ�ϸ���ֵһ���������ǣ�ƫ����
short ax,ay,az,gx,gy,gz;							//���ٶȣ������ǽ��ٶ�


int main(void)
{

	Setup();
	
	//�Ƕ�PID
	while(!Key_GetNum())
	{

		OLED_ShowSignedNum(1,1,Yaw,5);
		OLED_ShowSignedNum(2,1,wytest,5);
		Delay_ms(5);
	}
	AngleLeft.target=Yaw;
	AngleRight.target=Yaw;
	
	while (1)
	{
		//�ٶ�PID
		/*
			if(grey_scancal(&GREY_INDEX)!=2){
			grey_recal(&GREY_INDEX);
			GreyRight.current=GREY_INDEX;
			GreyLeft.current=GREY_INDEX;
			DrivePidCalc(&GreyRight,&GreyLeft,&SpeedR,&SpeedL);
			drive_setPWM34(SpeedR,SpeedL);
		}
		*/
		

		OLED_ShowBinNum(2,1,Read_GREY(),8);
		Delay_ms(5);
		
	}

}


