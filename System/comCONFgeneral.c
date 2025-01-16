#include "comCONFgeneral.h"

USART_TypeDef*  USART_printf=USART1;
uint8_t Serial_TxPacket[4];				//���巢�����ݰ����飬���ݰ���ʽ��FF 01 02 03 04 FE
uint8_t Serial_RxPacket[4];				//����������ݰ�����
uint8_t Serial_RxFlag;					//����������ݰ���־λ

void USART_setMODE(USART_TypeDef* USARTx,uint32_t BR)
{
		
	if(USARTx==USART1){
		PIN_setMODE(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP);
		PIN_setMODE(GPIOA,GPIO_Pin_10,GPIO_Mode_IPU);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//����USART1��ʱ��

	}
	else if(USARTx==USART2){
		PIN_setMODE(GPIOA,GPIO_Pin_2,GPIO_Mode_AF_PP);
		PIN_setMODE(GPIOA,GPIO_Pin_3,GPIO_Mode_IPU);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//����USART1��ʱ��
	}
	else if(USARTx==USART3){
		PIN_setMODE(GPIOB,GPIO_Pin_10,GPIO_Mode_AF_PP);
		PIN_setMODE(GPIOB,GPIO_Pin_11,GPIO_Mode_IPU);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//����USART1��ʱ��
	}
	
		/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = BR;				//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(USART1, &USART_InitStructure);				//���ṹ���������USART_Init������USART1

}

/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void SERIAL_sendBYTE(USART_TypeDef* USARTx,uint8_t Byte)
{
	USART_SendData(USARTx, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void SERIAL_sendARRAY(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		SERIAL_sendBYTE(USARTx,Array[i]);		//���ε���SERIAL_sendBYTE����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void SERIAL_sendSTRING(USART_TypeDef* USARTx,char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		SERIAL_sendBYTE(USARTx,String[i]);		//���ε���SERIAL_sendBYTE����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//���ý����ֵΪ1
	while (Y --)			//ִ��Y��
	{
		Result *= X;		//��X�۳˵����
	}
	return Result;
}

/**
  * ��    �������ڷ�������
  * ��    ����Number Ҫ���͵����֣���Χ��0~4294967295
  * ��    ����Length Ҫ�������ֵĳ��ȣ���Χ��0~10
  * �� �� ֵ����
  */
void SERIAL_sendNUM(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		SERIAL_sendBYTE(USARTx,Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//���ε���SERIAL_sendBYTE����ÿλ����
	}
}

/**
  * ��    ����ʹ��printf��Ҫ�ض���ĵײ㺯��
  * ��    ��������ԭʼ��ʽ���ɣ�����䶯
  * �� �� ֵ������ԭʼ��ʽ���ɣ�����䶯
  */
int fputc(int ch, FILE *f)
{
	SERIAL_sendBYTE(USART_printf,ch);			//��printf�ĵײ��ض����Լ��ķ����ֽں���
	return ch;
}

/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void Serial_Printf(USART_TypeDef* USARTx,char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	SERIAL_sendSTRING(USARTx,String);		//���ڷ����ַ����飨�ַ�����
}

/**
  * ��    �������ڷ������ݰ�
  * ��    ������
  * �� �� ֵ����
  * ˵    �������ô˺�����Serial_TxPacket��������ݽ����ϰ�ͷ��FF����β��FE������Ϊ���ݰ����ͳ�ȥ
  */
void Serial_SendPacket(USART_TypeDef* USARTx)
{
	SERIAL_sendBYTE(USARTx,0xFF);
	SERIAL_sendARRAY(USARTx,Serial_TxPacket, 4);
	SERIAL_sendBYTE(USARTx,0xFE);
}

/**
  * ��    ������ȡ���ڽ������ݰ���־λ
  * ��    ������
  * �� �� ֵ�����ڽ������ݰ���־λ����Χ��0~1�����յ����ݰ��󣬱�־λ��1����ȡ���־λ�Զ�����
  */
uint8_t Serial_GetRxFlag(USART_TypeDef* USARTx)
{
	if (Serial_RxFlag == 1)			//�����־λΪ1
	{
		Serial_RxFlag = 0;
		return 1;					//�򷵻�1�����Զ������־λ
	}
	return 0;						//�����־λΪ0���򷵻�0
}

//IIC setting
		void IIC_setGROUP(IIC_group* IIC_GROUP,GPIO_TypeDef* pinGroupSCL,
			uint16_t pinSCL,GPIO_TypeDef* pinGroupSDA,uint16_t pinSDA)
		{
			PIN_setMODE(pinGroupSCL,pinSCL,GPIO_Mode_Out_OD);
			PIN_setMODE(pinGroupSDA,pinSDA,GPIO_Mode_Out_OD);
			
			IIC_GROUP->pinGroupSCL=pinGroupSCL;
			IIC_GROUP->pinGroupSDA=pinGroupSDA;
			IIC_GROUP->pinSCL=pinSCL;
			IIC_GROUP->pinSDA=pinSDA;
			
			PIN_writeBIT(pinGroupSCL,pinSCL,_HIGH);
			PIN_writeBIT(pinGroupSDA,pinSDA,_HIGH);
		}

void IIC_W_SCL(IIC_group g,uint8_t b)
{
	GPIO_WriteBit(g.pinGroupSCL, g.pinSCL, (BitAction)b);		//����BitValue������SDA���ŵĵ�ƽ��BitValueҪʵ�ַ�0��1������
	Delay_us(10);	
}

void IIC_W_SDA(IIC_group g,uint8_t b)
{
	GPIO_WriteBit(g.pinGroupSDA, g.pinSDA, (BitAction)b);		//����BitValue������SDA���ŵĵ�ƽ��BitValueҪʵ�ַ�0��1������
	Delay_us(10);	
}
uint8_t IIC_R_SDA(IIC_group g)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(g.pinGroupSDA, g.pinSDA);		//��ȡSDA��ƽ
	Delay_us(10);												//��ʱ10us����ֹʱ��Ƶ�ʳ���Ҫ��
	return BitValue;											//����SDA��ƽ
}


/*Э���*/

/**
  * ��    ����I2C��ʼ
  * ��    ������
  * �� �� ֵ����
  */
void IIC_Start(IIC_group g)
{
	IIC_W_SDA(g,1);							//�ͷ�SDA��ȷ��SDAΪ�ߵ�ƽ
	IIC_W_SCL(g,1);							//�ͷ�SCL��ȷ��SCLΪ�ߵ�ƽ
	IIC_W_SDA(g,0);							//��SCL�ߵ�ƽ�ڼ䣬����SDA��������ʼ�ź�
	IIC_W_SCL(g,0);							//��ʼ���SCLҲ���ͣ���Ϊ��ռ�����ߣ�ҲΪ�˷�������ʱ���ƴ��
}

/**
  * ��    ����I2C��ֹ
  * ��    ������
  * �� �� ֵ����
  */
void IIC_Stop(IIC_group g)
{
	IIC_W_SDA(g,0);							//����SDA��ȷ��SDAΪ�͵�ƽ
	IIC_W_SCL(g,1);							//�ͷ�SCL��ʹSCL���ָߵ�ƽ
	IIC_W_SDA(g,1);							//��SCL�ߵ�ƽ�ڼ䣬�ͷ�SDA��������ֹ�ź�
}

/**
  * ��    ����I2C����һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ����ݣ���Χ��0x00~0xFF
  * �� �� ֵ����
  */
void IIC_SendByte(IIC_group g,uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				//ѭ��8�Σ��������η������ݵ�ÿһλ
	{
		IIC_W_SDA(g,Byte & (0x80 >> i));	//ʹ������ķ�ʽȡ��Byte��ָ��һλ���ݲ�д�뵽SDA��
		IIC_W_SCL(g,1);						//�ͷ�SCL���ӻ���SCL�ߵ�ƽ�ڼ��ȡSDA
		IIC_W_SCL(g,0);						//����SCL��������ʼ������һλ����
	}
}

/**
  * ��    ����I2C����һ���ֽ�
  * ��    ������
  * �� �� ֵ�����յ���һ���ֽ����ݣ���Χ��0x00~0xFF
  */
uint8_t IIC_ReceiveByte(IIC_group g)
{
	uint8_t i, Byte = 0x00;					//������յ����ݣ�������ֵ0x00���˴����븳��ֵ0x00��������õ�
	IIC_W_SDA(g,1);							//����ǰ��������ȷ���ͷ�SDA��������Ŵӻ������ݷ���
	for (i = 0; i < 8; i ++)				//ѭ��8�Σ��������ν������ݵ�ÿһλ
	{
		IIC_W_SCL(g,1);						//�ͷ�SCL����������SCL�ߵ�ƽ�ڼ��ȡSDA
		if (IIC_R_SDA(g) == 1){Byte |= (0x80 >> i);}	//��ȡSDA���ݣ����洢��Byte����
														//��SDAΪ1ʱ���ñ���ָ��λΪ1����SDAΪ0ʱ����������ָ��λΪĬ�ϵĳ�ֵ0
		IIC_W_SCL(g,0);						//����SCL���ӻ���SCL�͵�ƽ�ڼ�д��SDA
	}
	return Byte;							//���ؽ��յ���һ���ֽ�����
}

/**
  * ��    ����I2C����Ӧ��λ
  * ��    ����Byte Ҫ���͵�Ӧ��λ����Χ��0~1��0��ʾӦ��1��ʾ��Ӧ��
  * �� �� ֵ����
  */
void IIC_SendAck(IIC_group g,uint8_t AckBit)
{
	IIC_W_SDA(g,AckBit);					//������Ӧ��λ���ݷŵ�SDA��
	IIC_W_SCL(g,1);							//�ͷ�SCL���ӻ���SCL�ߵ�ƽ�ڼ䣬��ȡӦ��λ
	IIC_W_SCL(g,0);							//����SCL����ʼ��һ��ʱ��ģ��
}

/**
  * ��    ����I2C����Ӧ��λ
  * ��    ������
  * �� �� ֵ�����յ���Ӧ��λ����Χ��0~1��0��ʾӦ��1��ʾ��Ӧ��
  */
uint8_t IIC_ReceiveAck(IIC_group g)
{
	uint8_t AckBit;							//����Ӧ��λ����
	IIC_W_SDA(g,1);							//����ǰ��������ȷ���ͷ�SDA��������Ŵӻ������ݷ���
	IIC_W_SCL(g,1);							//�ͷ�SCL����������SCL�ߵ�ƽ�ڼ��ȡSDA
	AckBit = IIC_R_SDA(g);					//��Ӧ��λ�洢��������
	IIC_W_SCL(g,0);							//����SCL����ʼ��һ��ʱ��ģ��
	return AckBit;							//���ض���Ӧ��λ����
}







