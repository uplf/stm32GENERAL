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