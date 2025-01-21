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
#include "grey.h"
#include "userSetup.h"
#include "W25Q64.h"

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
	GREY_init();
	W25Q64_Init();

}


uint8_t rx[]={1,2,3};
uint8_t wx[3];
int main(void)
{

	Setup();
	W25Q64_ReadData(0x00,wx,3);
	while (1)
	{
		OLED_ShowNum(1,1,wx[0],3);
		OLED_ShowNum(2,1,wx[1],3);
		OLED_ShowNum(3,1,wx[2],3);
	}

}


