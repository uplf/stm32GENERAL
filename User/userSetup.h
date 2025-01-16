#ifndef __PIN_SETUP
#define __PIN_SETUP

/*******Section0:ATTENTION
1.<PIN>SPI communication is fixed attached to the following pins, which means these pins can't
	be assigned to other usage once SPI is activated
	PA4 PA5 PA6 PA7
*/

/*******Section1:basic hardware Setup
1.OLED_PINS
*/

//OLED_PINS
#define OLED_W_SCL_Pin GPIO_Pin_0
#define OLED_W_SDA_Pin GPIO_Pin_1
#define OLED_W_Port GPIOA

#endif
