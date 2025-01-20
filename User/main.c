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

//end of test area
int status=0;
uint8_t x[]={0x1,0x2,0x3,0x4};


void Setup(void){
	Encoder_Init();
	USART_setMODE(USART1,115200);
	USART_Cmd(USART1,ENABLE);
	INT_setNVIC(usartINT_set(USART1),1,1,ENABLE);
	reuse_init();
	OLED_Init();
	drive_init();
	MPU6050_Init();


	//SERIAL_sendNUM(USART1,1,4);
}

short A,B,C;

int main(void)
{
	Setup();
	while (1)
	{
		MPU_Get_Gyroscope(&A,&B,&C);
		OLED_ShowSignedNum(1,1,A,5);
		OLED_ShowSignedNum(2,1,B,5);
		OLED_ShowSignedNum(3,1,C,5);
		Delay_ms(500);
	}

}


