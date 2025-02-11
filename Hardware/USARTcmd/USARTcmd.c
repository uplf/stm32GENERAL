#include "USARTcmd.h"


uint8_t cmdUSART_TxPacket[cmdUSART_LENGTH];				//���巢�����ݰ����飬��
uint8_t cmdUSART_RxPacket[cmdUSART_LENGTH];				//����������ݰ�����
uint8_t cmdUSART_RxFlag;					//����������ݰ���־λ

uint8_t cycleUSART_DARA1addr;
uint8_t cycleUSART_DARA2addr;
uint8_t cycleUSART_DARA3addr;



void USART1_IRQHandler(void)
{

	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART1�Ľ����¼��������ж�
	{

		uint8_t RxData = USART_ReceiveData(USART1);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���

		

		if (RxState == 0)
		{
			if (RxData == 0xFF)			//�������ȷʵ�ǰ�ͷ
			{

				RxState = 1;			//����һ��״̬
				pRxPacket = 0;			//���ݰ���λ�ù���
			}
		}

		else if (RxState == 1)
		{
			cmdUSART_RxPacket[pRxPacket] = RxData;	//�����ݴ������ݰ������ָ��λ��
			pRxPacket ++;				//���ݰ���λ������
			if (pRxPacket >= cmdUSART_LENGTH)			//����չ�4������
			{
				RxState = 2;			//����һ��״̬
			}
		}

		else if (RxState == 2)
		{
			if (RxData == 0xFE)			//�������ȷʵ�ǰ�β��
			{
				RxState = 0;			//״̬��0
				cmdUSART_RxFlag = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//�����־λ

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
			/*��������ע��
			status_tmp��������¼������ģ�
			proc��ʱָ����Ҫִ�еĺ���
			����1Ϊcount���Ӧ������/��������
			����2Ϊcount���Ӧ�����ݺŵ�����uint8_t�����
			����3��count��ţ��ڲ������count���ŷ�λ�Σ�ͬʱ�����count����д�ظ�
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

