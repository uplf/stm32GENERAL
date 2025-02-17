#ifndef __GREY_H
#define __GREY_H

void GREY_init();
uint8_t GREY_ReadReg(uint8_t);
void GREY_WriteReg(uint8_t, uint8_t);
uint8_t Read_GREY();
uint8_t grey_scancal(int8_t* tmp);

void grey_recal(int8_t* tmp);

#endif
