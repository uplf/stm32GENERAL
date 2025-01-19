#ifndef __COM_CONF_GENERAL
#define __COM_CONF_GENERAL

#include "CONFgeneral.h"
#include "Delay.h"
#include <stdio.h>
#include <stdarg.h>


//printf redirect
extern USART_TypeDef*  USART_printf;


/******Section5:Communication
1.usart
2.iic
3.spi
*/

	
	//section5
extern uint8_t Serial_TxPacket[4];				//定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
extern uint8_t Serial_RxPacket[4];				//定义接收数据包数组
extern uint8_t Serial_RxFlag;					//定义接收数据包标志位

	//part1:serial setting
		//step1:init
		void USART_setMODE(USART_TypeDef* USARTx,uint32_t BR);
		//step2:(**)INT conf
		//step3:USART_Cmd(USARTx, ENABLE);
		//stepx:use
		void SERIAL_sendBYTE(USART_TypeDef* USARTx,uint8_t Byte);
		void SERIAL_sendARRAY(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length);
		void SERIAL_sendSTRING(USART_TypeDef* USARTx,char *String);
		uint32_t Serial_Pow(uint32_t X, uint32_t Y);
		void SERIAL_sendNUM(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length);
		int fputc(int ch, FILE *f);
		void Serial_Printf(USART_TypeDef* USARTx,char *format, ...);
		uint8_t Serial_GetRxFlag(USART_TypeDef* USARTx);
		 

	//part2:IIC setting
		//step1:create a object
		typedef struct {
			GPIO_TypeDef* pinGroupSCL;
			GPIO_TypeDef* pinGroupSDA;
			uint16_t pinSCL;
			uint16_t pinSDA;
				// ...
		} IIC_group;
		//step2:init the object by assigning pins
			void IIC_setGROUP(IIC_group* IIC_GROUP,GPIO_TypeDef* pinGroupSCL,
			uint16_t pinSCL,GPIO_TypeDef* pinGroupSDA,uint16_t pinSDA);
		//step3:create drive code with the following&<hwCONFIGgen.h> source
			void IIC_Start(IIC_group g);
			void IIC_Stop(IIC_group g);
			void IIC_SendByte(IIC_group g,uint8_t Byte);
			uint8_t IIC_ReceiveByte(IIC_group g);
			void IIC_SendAck(IIC_group g,uint8_t AckBit);
			uint8_t IIC_ReceiveAck(IIC_group g);
			void IIC_W_SCL(IIC_group g,uint8_t b);
			void IIC_W_SDA(IIC_group g,uint8_t b);
			uint8_t IIC_R_SDA(IIC_group g);
			
			
			
	//part3.SPI(hardware)setting

#endif
