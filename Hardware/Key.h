#ifndef __KEY_H
#define __KEY_H

extern int status;
extern int16_t Encoder_Count;	

void Key_Init(void);
uint8_t Key_GetNum(void);

void Encoder_Init(void);


#endif
