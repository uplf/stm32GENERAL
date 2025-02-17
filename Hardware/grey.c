#include "stm32f10x.h"                  // Device header
#include "comCONFgeneral.h"
#include "grey.h"
#include "userSetup.h"

#define GREY_ADDRESS 0x98


IIC_group g;
int8_t greyC,greyT=0;		//灰度传感器的现有参数和目标参数
uint8_t grey_dec;


void GREY_init(){
	IIC_setGROUP(&g,GREY_Port,GREY_SCL_PIN,GREY_Port,GREY_SDA_PIN);
}


void GREY_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	IIC_Start(g);						//I2C起始
	IIC_SendByte(g,GREY_ADDRESS);	//发送从机地址，读写位为0，表示即将写入
	IIC_ReceiveAck(g);					//接收应答
	IIC_SendByte(g,RegAddress);			//发送寄存器地址
	IIC_ReceiveAck(g);					//接收应答
	IIC_SendByte(g,Data);				//发送要写入寄存器的数据
	IIC_ReceiveAck(g);					//接收应答
	IIC_Stop(g);						//I2C终止
}

uint8_t GREY_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	IIC_Start(g);						//I2C起始
	IIC_SendByte(g,GREY_ADDRESS);	//发送从机地址，读写位为0，表示即将写入
	IIC_ReceiveAck(g);					//接收应答
	IIC_SendByte(g,RegAddress);			//发送寄存器地址
	IIC_ReceiveAck(g);				//接收应答
	
	IIC_Start(g);						//I2C重复起始
	IIC_SendByte(g,GREY_ADDRESS | 0x01);	//发送从机地址，读写位为1，表示即将读取
	IIC_ReceiveAck(g);					//接收应答
	Data = IIC_ReceiveByte(g);			//接收指定寄存器的数据
	IIC_SendAck(g,1);					//发送应答，给从机非应答，终止从机的数据输出
	IIC_Stop(g);						//I2C终止
	
	return Data;
}

uint8_t Read_GREY()
{
	uint8_t Data;
	
	IIC_Start(g);						//I2C重复起始
	IIC_SendByte(g,GREY_ADDRESS | 0x01);	//发送从机地址，读写位为1，表示即将读取
	IIC_ReceiveAck(g);					//接收应答
	Data = IIC_ReceiveByte(g);			//接收指定寄存器的数据
	IIC_SendAck(g,1);					//发送应答，给从机非应答，终止从机的数据输出
	IIC_Stop(g);						//I2C终止
	
	return Data;
}



//逻辑：AB区同时存在
uint8_t grey_scancal(int8_t* tmp)
{
	int8_t temp,flag=0;//count=0;
	//uint8_t grey_det[8];
	static int8_t outp;
	uint8_t m0=~GREY_ReadReg(0xDD);
	grey_dec=m0;
	if(m0==0)									//未检测到情况下
	{ 
		if(!outp)return 3;			//若上一个输出为0，报错
		*tmp=outp;							//否则保持上一个输出
		return 1;
	}
	//--=-----             0
	//=------- -> -------- 1
	
	if((m0&(~(0x18)))==0)			//若45位以外没有东西，则输出0
	{
		outp=0;
		*tmp=outp;
		return 0;
	}
	if((m0&0xF0)&&(m0&0x0F))		//若两侧都有东西，则输出0
	{
		outp=0;
		*tmp=outp;
		return 1;
	}

	if(m0&0x0F)								//其他情况按照最外侧所对应的值输出
	{
		//4,3,2,1
		for(flag=4;flag;flag--)
		{
			if(m0&0x01)
			{
				outp=flag;
				*tmp=outp;
				return 0;
			}
			else m0=m0>>1;
		}
	}
	if(m0&0xF0)
	{
		m0=m0>>4;
		//-1,-2,-3,-4
		for(flag=-1;flag>=-4;flag--)
		{
			temp=m0;
			if((temp>>((-flag)-1))&0x01)
			{
				outp=flag;
				*tmp=outp;
				return 0;
			}
		}
	}
	return 2;
}

void grey_recal(int8_t* tmp)
{
	if(*tmp>=1)*tmp-=1;
	if(*tmp<=-1)*tmp+=1;
	return;
}

//使用默认情况，不做初始化

//void GREY_GetData()
//{
//}