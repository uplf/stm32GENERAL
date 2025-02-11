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


//section2 串口调试
void USARTdebug_init(){
	USART_setMODE(USART1,115200);
	USART_Cmd(USART1,ENABLE);
	INT_setNVIC(usartINT_set(USART1),1,1,ENABLE);
}

/*
需要传输10个HEX包
####HEX1开头信号
	FF
####HEX2命令类型(0)
	00**根据已定义内容周期性发送数据包1
	01**单次发送数据包1
	02**停止周期性发送1
	03**单次发送存储模块的数据包
	04**根据下面内容写入暂存变量
	05**用存储数据定义内部变量
	06**发送数据直接存储于W25
	07**调用其他函数
（回复时第一位hex为1）
####HEX3命令编号(1)
	自定义，将用于回复
####HEX4-12（3*3）命令index(2-34,5-67,8-9.10)
新一轮
[一位]标识号
	00-希望发送的数据索引
	01-希望发送的数据索引
	03-希望存储的数据索引
	04-希望暂存的数据索引
	05-希望内定义的数据索引
	06-数据索引
	07-函数索引
	(回复第一位修改)
[二位三位]数据内容
8位数据，占用3位
16为数据，高位占用2位
####HEX10结尾信号
	FE
*/

