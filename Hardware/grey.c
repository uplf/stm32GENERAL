#include "stm32f10x.h"                  // Device header
#include "comCONFgeneral.h"
#include "grey.h"
#include "userSetup.h"

#define GREY_ADDRESS 0x98


IIC_group g;
int8_t greyC,greyT=0;		//�Ҷȴ����������в�����Ŀ�����
uint8_t grey_dec;


void GREY_init(){
	IIC_setGROUP(&g,GREY_Port,GREY_SCL_PIN,GREY_Port,GREY_SDA_PIN);
}


void GREY_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	IIC_Start(g);						//I2C��ʼ
	IIC_SendByte(g,GREY_ADDRESS);	//���ʹӻ���ַ����дλΪ0����ʾ����д��
	IIC_ReceiveAck(g);					//����Ӧ��
	IIC_SendByte(g,RegAddress);			//���ͼĴ�����ַ
	IIC_ReceiveAck(g);					//����Ӧ��
	IIC_SendByte(g,Data);				//����Ҫд��Ĵ���������
	IIC_ReceiveAck(g);					//����Ӧ��
	IIC_Stop(g);						//I2C��ֹ
}

uint8_t GREY_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	IIC_Start(g);						//I2C��ʼ
	IIC_SendByte(g,GREY_ADDRESS);	//���ʹӻ���ַ����дλΪ0����ʾ����д��
	IIC_ReceiveAck(g);					//����Ӧ��
	IIC_SendByte(g,RegAddress);			//���ͼĴ�����ַ
	IIC_ReceiveAck(g);				//����Ӧ��
	
	IIC_Start(g);						//I2C�ظ���ʼ
	IIC_SendByte(g,GREY_ADDRESS | 0x01);	//���ʹӻ���ַ����дλΪ1����ʾ������ȡ
	IIC_ReceiveAck(g);					//����Ӧ��
	Data = IIC_ReceiveByte(g);			//����ָ���Ĵ���������
	IIC_SendAck(g,1);					//����Ӧ�𣬸��ӻ���Ӧ����ֹ�ӻ����������
	IIC_Stop(g);						//I2C��ֹ
	
	return Data;
}

uint8_t Read_GREY()
{
	uint8_t Data;
	
	IIC_Start(g);						//I2C�ظ���ʼ
	IIC_SendByte(g,GREY_ADDRESS | 0x01);	//���ʹӻ���ַ����дλΪ1����ʾ������ȡ
	IIC_ReceiveAck(g);					//����Ӧ��
	Data = IIC_ReceiveByte(g);			//����ָ���Ĵ���������
	IIC_SendAck(g,1);					//����Ӧ�𣬸��ӻ���Ӧ����ֹ�ӻ����������
	IIC_Stop(g);						//I2C��ֹ
	
	return Data;
}



//�߼���AB��ͬʱ����
uint8_t grey_scancal(int8_t* tmp)
{
	int8_t temp,flag=0;//count=0;
	//uint8_t grey_det[8];
	static int8_t outp;
	uint8_t m0=~GREY_ReadReg(0xDD);
	grey_dec=m0;
	if(m0==0)									//δ��⵽�����
	{ 
		if(!outp)return 3;			//����һ�����Ϊ0������
		*tmp=outp;							//���򱣳���һ�����
		return 1;
	}
	//--=-----             0
	//=------- -> -------- 1
	
	if((m0&(~(0x18)))==0)			//��45λ����û�ж����������0
	{
		outp=0;
		*tmp=outp;
		return 0;
	}
	if((m0&0xF0)&&(m0&0x0F))		//�����඼�ж����������0
	{
		outp=0;
		*tmp=outp;
		return 1;
	}

	if(m0&0x0F)								//��������������������Ӧ��ֵ���
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

//ʹ��Ĭ�������������ʼ��

//void GREY_GetData()
//{
//}