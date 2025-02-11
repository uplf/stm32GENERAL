#include "USARTcmd.h"


uint8_t cmdUSART_TxPacket[cmdUSART_LENGTH];				//定义发送数据包数组，数
uint8_t cmdUSART_RxPacket[cmdUSART_LENGTH];				//定义接收数据包数组
uint8_t cmdUSART_RxFlag;					//定义接收数据包标志位

uint8_t cycleUSART_DARA1addr;
uint8_t cycleUSART_DARA2addr;
uint8_t cycleUSART_DARA3addr;



void USART1_IRQHandler(void)
{

	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
	{

		uint8_t RxData = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量

		

		if (RxState == 0)
		{
			if (RxData == 0xFF)			//如果数据确实是包头
			{

				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}

		else if (RxState == 1)
		{
			cmdUSART_RxPacket[pRxPacket] = RxData;	//将数据存入数据包数组的指定位置
			pRxPacket ++;				//数据包的位置自增
			if (pRxPacket >= cmdUSART_LENGTH)			//如果收够4个数据
			{
				RxState = 2;			//置下一个状态
			}
		}

		else if (RxState == 2)
		{
			if (RxData == 0xFE)			//如果数据确实是包尾部
			{
				RxState = 0;			//状态归0
				cmdUSART_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除标志位

	}
}



void cmdUSART_setMODE(){
	USART_setMODE(CMD_USART,115200);
	USART_Cmd(CMD_USART,ENABLE);
	INT_setNVIC(usartINT_set(USART1),1,1,ENABLE);
}


uint8_t packetRESOLVE(){
	uint8_t status_tmp,status=0;
	uint8_t (*proc)(uint8_t,uint16_t,uint8_t*,uint8_t);
		
	switch(cmdUSART_RxPacket[0]){
		case 00:{
			proc=&cycleDATA_sendF;
			break;
		}
		case 01:{
			proc=&singleDATA_sendF ;
			break;
		}
		case 02:{
			proc=&cycleDATA_stopF ;
			break;
		}
		case 03:{
			proc=&singleDATA_sendM ;
			break;
		}
		case 04:{
			proc=&singleDATA_writeF ;
			break;
		}
		case 05:{
			proc=&singleDATA_writeFM ;
			break;
		}
		case 06:{
			proc=&singleDATA_writeM ;
			break;
		}
		case 07:{
			proc=&singleFUNC ;
			break;
		}
		default:{
			
			
			return 3;
		}
	}
		cmdUSART_TxPacket[1]=cmdUSART_RxPacket[1];
		
		for(uint8_t count=1;count<=3;count++)
		{
			status_tmp=proc(cmdUSART_RxPacket[3*count-1],
			int8TOint16(cmdUSART_RxPacket+(3*count),cmdUSART_RxPacket+(3*count+1)),
			cmdUSART_TxPacket,count);
			/*函数调用注释
			status_tmp是用来记录错误码的，
			proc此时指向需要执行的函数
			参数1为count组对应的命令/变量索引
			参数2为count组对应的内容号的两个uint8_t的组合
			参数3是count序号，内部会根据count来排放位次，同时会根据count来书写回复
			*/
			
			status=(status_tmp>status)?status_tmp:status;
			
		}
		cmdUSART_TxPacket[0]=(status<<4)|cmdUSART_RxPacket[0];
		
		return status;
	}
	
	
uint8_t cycleDATA_sendF(uint8_t F_addr,uint16_t omit,uint8_t* TX,uint8_t count){
	
}

uint8_t singleDATA_sendF(uint8_t a,uint16_t b,uint8_t* c,uint8_t d){return 0;}
uint8_t cycleDATA_stopF(uint8_t a,uint16_t b,uint8_t* c,uint8_t d){return 0;}
uint8_t singleDATA_sendM(uint8_t a,uint16_t b,uint8_t* c,uint8_t d){return 0;}
uint8_t singleDATA_writeF(uint8_t a,uint16_t b,uint8_t* c,uint8_t d){return 0;}
uint8_t singleDATA_writeFM(uint8_t a,uint16_t b,uint8_t* c,uint8_t d){return 0;}
uint8_t singleDATA_writeM(uint8_t a,uint16_t b,uint8_t* c,uint8_t d){return 0;}
uint8_t singleFUNC(uint8_t a,uint16_t b,uint8_t* c,uint8_t d){return 0;}
	
	
	
	
	
	

void int16TOint8(uint8_t* lowByte,uint8_t* highByte,uint16_t inp){
	*lowByte=inp&0xFF;
	*highByte=((inp)>>8)&0xFF;
}

uint16_t int8TOint16(uint8_t* lowByte,uint8_t* highByte){
	return ((uint16_t)*highByte<<8)|((uint16_t)*lowByte);
}
double int8TOdouble(uint8_t* u1,uint8_t* u2,uint8_t* u3,uint8_t* u4){
}
void doubleTOint8(uint8_t* u1,uint8_t* u2,uint8_t* u3,uint8_t* u4,double d){
}

