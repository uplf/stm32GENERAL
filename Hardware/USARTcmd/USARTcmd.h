#ifndef __USART_CMD_H
#define __USART_CMD_H

/**
the lib is based on CONFgeneral.h
*/
#include "userSetup.h"
#include "comCONFgeneral.h"
#include "CONFgeneral.h"
#include "W25Q64.h"
#include <stdint.h>

#define cmdUSART_LENGTH 10 

extern uint8_t cmdUSART_TxPacket[cmdUSART_LENGTH];				//定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
extern uint8_t cmdUSART_RxPacket[cmdUSART_LENGTH];				//定义接收数据包数组
extern uint8_t cmdUSART_RxFlag;					//定义接收数据包标志位


/*****part2:
drive
*/
void cmdUSART_setMODE();

uint8_t packetRESOLVE();
//return suc->0, part fail->1, all fail->2, synax error->3


/*****part3:
function define
*/
uint8_t cycleDATA_sendF(uint8_t,uint16_t,uint8_t*,uint8_t);
uint8_t singleDATA_sendF(uint8_t,uint16_t,uint8_t*,uint8_t);
uint8_t cycleDATA_stopF(uint8_t,uint16_t,uint8_t*,uint8_t);
uint8_t singleDATA_sendM(uint8_t,uint16_t,uint8_t*,uint8_t);
uint8_t singleDATA_writeF(uint8_t,uint16_t,uint8_t*,uint8_t);
uint8_t singleDATA_writeFM(uint8_t,uint16_t,uint8_t*,uint8_t);
uint8_t singleDATA_writeM(uint8_t,uint16_t,uint8_t*,uint8_t);
uint8_t singleFUNC(uint8_t,uint16_t,uint8_t*,uint8_t);
//1.addr 2.ind 3.Tx addr 4.count

/******part4:
middle-func & map
*/
void int16TOint8(uint8_t*,uint8_t*,uint16_t);
uint16_t int8TOint16(uint8_t*,uint8_t*);
double int8TOdouble(uint8_t*,uint8_t*,uint8_t*,uint8_t*);
void doubleTOint8(uint8_t*,uint8_t*,uint8_t*,uint8_t*,double);


//W25Q64addr
#define PID_GpL 0x0002
#define PID_GiL 0x0003
#define PID_GdL 0x0004
#define PID_GpR 0x0005
#define PID_GiR 0x0006
#define PID_GdR 0x0007
#define PID_ApL 0x0008
//...


#endif
