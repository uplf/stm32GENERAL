#include "hwCONFIGgen.h"


void I2C_writeREG(IIC_group g,uint8_t slaveADDR, uint8_t regADDR,uint8_t Data)
{
		IIC_Start(g);						//I2C起始
	IIC_SendByte(g,slaveADDR);	//发送从机地址，读写位为0，表示即将写入
	IIC_ReceiveAck(g);					//接收应答
	IIC_SendByte(g,regADDR);			//发送寄存器地址
	IIC_ReceiveAck(g);					//接收应答
	IIC_SendByte(g,Data);				//发送要写入寄存器的数据
	IIC_ReceiveAck(g);					//接收应答
	IIC_Stop(g);						//I2C终止
}

uint8_t readREG(IIC_group g,uint8_t slaveADDR,uint8_t regADDR)
{
	uint8_t Data;
	
	IIC_Start(g);						//I2C起始
	IIC_SendByte(g,slaveADDR);	//发送从机地址，读写位为0，表示即将写入
	IIC_ReceiveAck(g);					//接收应答
	IIC_SendByte(g,regADDR);			//发送寄存器地址
	IIC_ReceiveAck(g);					//接收应答
	
	IIC_Start(g);						//I2C重复起始
	IIC_SendByte(g,slaveADDR | 0x01);	//发送从机地址，读写位为1，表示即将读取
	IIC_ReceiveAck(g);					//接收应答
	Data = IIC_ReceiveByte(g);			//接收指定寄存器的数据
	IIC_SendAck(g,1);					//发送应答，给从机非应答，终止从机的数据输出
	IIC_Stop(g);						//I2C终止
	
	return Data;
}