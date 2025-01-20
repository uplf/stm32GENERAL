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

	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t pRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART1�Ľ����¼��������ж�
	{

		uint8_t RxData = USART_ReceiveData(USART1);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		/*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
		
		/*��ǰ״̬Ϊ0���������ݰ���ͷ*/
		if (RxState == 0)
		{
			if (RxData == 0xFF)			//�������ȷʵ�ǰ�ͷ
			{

				RxState = 1;			//����һ��״̬
				pRxPacket = 0;			//���ݰ���λ�ù���
			}
		}
		/*��ǰ״̬Ϊ1���������ݰ�����*/
		else if (RxState == 1)
		{
			Serial_RxPacket[pRxPacket] = RxData;	//�����ݴ������ݰ������ָ��λ��
			pRxPacket ++;				//���ݰ���λ������
			if (pRxPacket >= 4)			//����չ�4������
			{
				RxState = 2;			//����һ��״̬
			}
		}
		/*��ǰ״̬Ϊ2���������ݰ���β*/
		else if (RxState == 2)
		{
			if (RxData == 0xFE)			//�������ȷʵ�ǰ�β��
			{
				//SERIAL_sendBYTE(USART1,9);
				RxState = 0;			//״̬��0
				Serial_RxFlag = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
			}
		}
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//�����־λ

	}
}


