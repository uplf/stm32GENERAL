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
#define OLED_W_SCL_Pin GPIO_Pin_3
#define OLED_W_SDA_Pin GPIO_Pin_4
#define OLED_W_Port GPIOB

//2.TB6612 conf_PINS
#define DRIVE_AIN1_GROUP GPIOB
#define DRIVE_AIN2_GROUP GPIOB
#define DRIVE_BIN1_GROUP GPIOB
#define DRIVE_BIN2_GROUP GPIOB
#define DRIVE_AIN1_PIN GPIO_Pin_14
#define DRIVE_AIN2_PIN GPIO_Pin_15
#define DRIVE_BIN1_PIN GPIO_Pin_13
#define DRIVE_BIN2_PIN GPIO_Pin_12

//3.CONFIG
#define REUSE_INIT 1

/*******Section2:index Setup
1.Speed*/
#define BASIC_SPEED 50
//1-100
#define ALTER_UNIT 1
#define INV_ABLE 1


#endif
