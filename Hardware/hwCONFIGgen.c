#include "hwCONFIGgen.h"


void I2C_writeREG(IIC_group g,uint8_t slaveADDR, uint8_t regADDR,uint8_t Data)
{
		IIC_Start(g);						//I2C��ʼ
	IIC_SendByte(g,slaveADDR);	//���ʹӻ���ַ����дλΪ0����ʾ����д��
	IIC_ReceiveAck(g);					//����Ӧ��
	IIC_SendByte(g,regADDR);			//���ͼĴ�����ַ
	IIC_ReceiveAck(g);					//����Ӧ��
	IIC_SendByte(g,Data);				//����Ҫд��Ĵ���������
	IIC_ReceiveAck(g);					//����Ӧ��
	IIC_Stop(g);						//I2C��ֹ
}

uint8_t readREG(IIC_group g,uint8_t slaveADDR,uint8_t regADDR)
{
	uint8_t Data;
	
	IIC_Start(g);						//I2C��ʼ
	IIC_SendByte(g,slaveADDR);	//���ʹӻ���ַ����дλΪ0����ʾ����д��
	IIC_ReceiveAck(g);					//����Ӧ��
	IIC_SendByte(g,regADDR);			//���ͼĴ�����ַ
	IIC_ReceiveAck(g);					//����Ӧ��
	
	IIC_Start(g);						//I2C�ظ���ʼ
	IIC_SendByte(g,slaveADDR | 0x01);	//���ʹӻ���ַ����дλΪ1����ʾ������ȡ
	IIC_ReceiveAck(g);					//����Ӧ��
	Data = IIC_ReceiveByte(g);			//����ָ���Ĵ���������
	IIC_SendAck(g,1);					//����Ӧ�𣬸��ӻ���Ӧ����ֹ�ӻ����������
	IIC_Stop(g);						//I2C��ֹ
	
	return Data;
}


//section2 ���ڵ���
void USARTdebug_init(){
	USART_setMODE(USART1,115200);
	USART_Cmd(USART1,ENABLE);
	INT_setNVIC(usartINT_set(USART1),1,1,ENABLE);
}

/*
��Ҫ����10��HEX��
####HEX1��ͷ�ź�
	FF
####HEX2��������(0)
	00**�����Ѷ������������Է������ݰ�1
	01**���η������ݰ�1
	02**ֹͣ�����Է���1
	03**���η��ʹ洢ģ������ݰ�
	04**������������д���ݴ����
	05**�ô洢���ݶ����ڲ�����
	06**��������ֱ�Ӵ洢��W25
	07**������������
���ظ�ʱ��һλhexΪ1��
####HEX3������(1)
	�Զ��壬�����ڻظ�
####HEX4-12��3*3������index(2-34,5-67,8-9.10)
��һ��
[һλ]��ʶ��
	00-ϣ�����͵���������
	01-ϣ�����͵���������
	03-ϣ���洢����������
	04-ϣ���ݴ����������
	05-ϣ���ڶ������������
	06-��������
	07-��������
	(�ظ���һλ�޸�)
[��λ��λ]��������
8λ���ݣ�ռ��3λ
16Ϊ���ݣ���λռ��2λ
####HEX10��β�ź�
	FE
*/

