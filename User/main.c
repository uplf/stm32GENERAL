#include "stm32f10x.h"                  // Device header

#include "Delay.h"
#include "OLED.h"
#include "CONFgeneral.h"
#include "comCONFgeneral.h"
#include "hwCONFIGgen.h"
#include "drive.h"
#include "key.h"
#include "map.h"

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
	Serial_TxPacket[0]=0x04;
	Serial_TxPacket[1]=0x01;
	Serial_TxPacket[2]=0x02;
	Serial_TxPacket[3]=0x03;
Delay_ms(1000);

	//SERIAL_sendNUM(USART1,1,4);
}

int main(void)
{
	Setup();
	while (1)
	{
	OLED_ShowChar(4,1,'m');
		SERIAL_sendARRAY(USART1,Serial_RxPacket,4);
		Delay_ms(1000);
				

	}

}


void USART1_IRQHandler(void)
{

	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
	{

		uint8_t RxData = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据包包头*/
		if (RxState == 0)
		{
			if (RxData == 0xFF)			//如果数据确实是包头
			{

				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}
		/*当前状态为1，接收数据包数据*/
		else if (RxState == 1)
		{
			Serial_RxPacket[pRxPacket] = RxData;	//将数据存入数据包数组的指定位置
			pRxPacket ++;				//数据包的位置自增
			if (pRxPacket >= 4)			//如果收够4个数据
			{
				RxState = 2;			//置下一个状态
			}
		}
		/*当前状态为2，接收数据包包尾*/
		else if (RxState == 2)
		{
			if (RxData == 0xFE)			//如果数据确实是包尾部
			{
				//SERIAL_sendBYTE(USART1,9);
				RxState = 0;			//状态归0
				Serial_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
			}
		}
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除标志位

	}
}


