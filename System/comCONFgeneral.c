#include "comCONFgeneral.h"

USART_TypeDef*  USART_printf=USART1;
uint8_t Serial_TxPacket[4];				//定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
uint8_t Serial_RxPacket[4];				//定义接收数据包数组
uint8_t Serial_RxFlag;					//定义接收数据包标志位

void USART_setMODE(USART_TypeDef* USARTx,uint32_t BR)
{
		
	if(USARTx==USART1){
		PIN_setMODE(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP);
		PIN_setMODE(GPIOA,GPIO_Pin_10,GPIO_Mode_IPU);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//开启USART1的时钟

	}
	else if(USARTx==USART2){
		PIN_setMODE(GPIOA,GPIO_Pin_2,GPIO_Mode_AF_PP);
		PIN_setMODE(GPIOA,GPIO_Pin_3,GPIO_Mode_IPU);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//开启USART1的时钟
	}
	else if(USARTx==USART3){
		PIN_setMODE(GPIOB,GPIO_Pin_10,GPIO_Mode_AF_PP);
		PIN_setMODE(GPIOB,GPIO_Pin_11,GPIO_Mode_IPU);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//开启USART1的时钟
	}
	
		/*USART初始化*/
	USART_InitTypeDef USART_InitStructure;					//定义结构体变量
	USART_InitStructure.USART_BaudRate = BR;				//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
	USART_Init(USART1, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1

}

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void SERIAL_sendBYTE(USART_TypeDef* USARTx,uint8_t Byte)
{
	USART_SendData(USARTx, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void SERIAL_sendARRAY(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		SERIAL_sendBYTE(USARTx,Array[i]);		//依次调用SERIAL_sendBYTE发送每个字节数据
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void SERIAL_sendSTRING(USART_TypeDef* USARTx,char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		SERIAL_sendBYTE(USARTx,String[i]);		//依次调用SERIAL_sendBYTE发送每个字节数据
	}
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}

/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void SERIAL_sendNUM(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		SERIAL_sendBYTE(USARTx,Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//依次调用SERIAL_sendBYTE发送每位数字
	}
}

/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
	SERIAL_sendBYTE(USART_printf,ch);			//将printf的底层重定向到自己的发送字节函数
	return ch;
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(USART_TypeDef* USARTx,char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	SERIAL_sendSTRING(USARTx,String);		//串口发送字符数组（字符串）
}

/**
  * 函    数：串口发送数据包
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，Serial_TxPacket数组的内容将加上包头（FF）包尾（FE）后，作为数据包发送出去
  */
void Serial_SendPacket(USART_TypeDef* USARTx)
{
	SERIAL_sendBYTE(USARTx,0xFF);
	SERIAL_sendARRAY(USARTx,Serial_TxPacket, 4);
	SERIAL_sendBYTE(USARTx,0xFE);
}

/**
  * 函    数：获取串口接收数据包标志位
  * 参    数：无
  * 返 回 值：串口接收数据包标志位，范围：0~1，接收到数据包后，标志位置1，读取后标志位自动清零
  */
uint8_t Serial_GetRxFlag(USART_TypeDef* USARTx)
{
	if (Serial_RxFlag == 1)			//如果标志位为1
	{
		Serial_RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
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
	GPIO_WriteBit(g.pinGroupSCL, g.pinSCL, (BitAction)b);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	Delay_us(10);	
}

void IIC_W_SDA(IIC_group g,uint8_t b)
{
	GPIO_WriteBit(g.pinGroupSDA, g.pinSDA, (BitAction)b);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	Delay_us(10);	
}
uint8_t IIC_R_SDA(IIC_group g)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(g.pinGroupSDA, g.pinSDA);		//读取SDA电平
	Delay_us(10);												//延时10us，防止时序频率超过要求
	return BitValue;											//返回SDA电平
}


/*协议层*/

/**
  * 函    数：I2C起始
  * 参    数：无
  * 返 回 值：无
  */
void IIC_Start(IIC_group g)
{
	IIC_W_SDA(g,1);							//释放SDA，确保SDA为高电平
	IIC_W_SCL(g,1);							//释放SCL，确保SCL为高电平
	IIC_W_SDA(g,0);							//在SCL高电平期间，拉低SDA，产生起始信号
	IIC_W_SCL(g,0);							//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/**
  * 函    数：I2C终止
  * 参    数：无
  * 返 回 值：无
  */
void IIC_Stop(IIC_group g)
{
	IIC_W_SDA(g,0);							//拉低SDA，确保SDA为低电平
	IIC_W_SCL(g,1);							//释放SCL，使SCL呈现高电平
	IIC_W_SDA(g,1);							//在SCL高电平期间，释放SDA，产生终止信号
}

/**
  * 函    数：I2C发送一个字节
  * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void IIC_SendByte(IIC_group g,uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				//循环8次，主机依次发送数据的每一位
	{
		IIC_W_SDA(g,Byte & (0x80 >> i));	//使用掩码的方式取出Byte的指定一位数据并写入到SDA线
		IIC_W_SCL(g,1);						//释放SCL，从机在SCL高电平期间读取SDA
		IIC_W_SCL(g,0);						//拉低SCL，主机开始发送下一位数据
	}
}

/**
  * 函    数：I2C接收一个字节
  * 参    数：无
  * 返 回 值：接收到的一个字节数据，范围：0x00~0xFF
  */
uint8_t IIC_ReceiveByte(IIC_group g)
{
	uint8_t i, Byte = 0x00;					//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	IIC_W_SDA(g,1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)				//循环8次，主机依次接收数据的每一位
	{
		IIC_W_SCL(g,1);						//释放SCL，主机机在SCL高电平期间读取SDA
		if (IIC_R_SDA(g) == 1){Byte |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
														//当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
		IIC_W_SCL(g,0);						//拉低SCL，从机在SCL低电平期间写入SDA
	}
	return Byte;							//返回接收到的一个字节数据
}

/**
  * 函    数：I2C发送应答位
  * 参    数：Byte 要发送的应答位，范围：0~1，0表示应答，1表示非应答
  * 返 回 值：无
  */
void IIC_SendAck(IIC_group g,uint8_t AckBit)
{
	IIC_W_SDA(g,AckBit);					//主机把应答位数据放到SDA线
	IIC_W_SCL(g,1);							//释放SCL，从机在SCL高电平期间，读取应答位
	IIC_W_SCL(g,0);							//拉低SCL，开始下一个时序模块
}

/**
  * 函    数：I2C接收应答位
  * 参    数：无
  * 返 回 值：接收到的应答位，范围：0~1，0表示应答，1表示非应答
  */
uint8_t IIC_ReceiveAck(IIC_group g)
{
	uint8_t AckBit;							//定义应答位变量
	IIC_W_SDA(g,1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	IIC_W_SCL(g,1);							//释放SCL，主机机在SCL高电平期间读取SDA
	AckBit = IIC_R_SDA(g);					//将应答位存储到变量里
	IIC_W_SCL(g,0);							//拉低SCL，开始下一个时序模块
	return AckBit;							//返回定义应答位变量
}







