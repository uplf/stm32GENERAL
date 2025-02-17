#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "CONFgeneral.h"
#include "userSetup.h"
#include "OLED.h"
#include "key.h"
#include "comCONFgeneral.h"
/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
	int key_num=0;
	
void Key_Init(void)
{
	PIN_setMODE(KEY_1_Port,KEY_1_PIN,GPIO_Mode_IPU);
	PIN_setMODE(KEY_2_Port,KEY_2_PIN,GPIO_Mode_IPU);
	PIN_setMODE(KEY_3_Port,KEY_3_PIN,GPIO_Mode_IPU);

}



/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;		//定义变量，默认键码值为0
	
	if (GPIO_ReadInputDataBit(KEY_1_Port, KEY_1_PIN) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(KEY_1_Port, KEY_1_PIN) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 1;												//置键码为1
	}
	
	if (GPIO_ReadInputDataBit(KEY_2_Port, KEY_2_PIN) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键2按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(KEY_2_Port, KEY_2_PIN) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 2;												//置键码为2
	}
	if (GPIO_ReadInputDataBit(KEY_3_Port, KEY_3_PIN) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键2按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(KEY_3_Port, KEY_3_PIN) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 3;												//置键码为2
	}
	
	return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}





int16_t Encoder_Count=0;					//全局变量，用于计数旋转编码器的增量值

/**
  * 函    数：旋转编码器初始化
  * 参    数：无
  * 返 回 值：无
  */
void Encoder_Init(void)
{

	PIN_setMODE(ENCODER_Port,ENCODERa_PIN|ENCODERb_PIN,GPIO_Mode_IPU);
	//PriorityGroup
	EXTI_setGROUP(NVIC_PriorityGroup_2);
	INT_setNVIC(gpioINT_setEXTI(ENCODER_Port,ENCODERa_PIN,EXTI_Trigger_Falling),1,1,ENABLE);
	INT_setNVIC(gpioINT_setEXTI(ENCODER_Port,ENCODERb_PIN,EXTI_Trigger_Falling),1,2,ENABLE);	

}

/**
  * 函    数：旋转编码器获取增量值
  * 参    数：无
  * 返 回 值：自上此调用此函数后，旋转编码器的增量值
  */
int16_t Encoder_Get(void)
{
	/*使用Temp变量作为中继，目的是返回Encoder_Count后将其清零*/
	/*在这里，也可以直接返回Encoder_Count
	  但这样就不是获取增量值的操作方法了
	  也可以实现功能，只是思路不一样*/
	int16_t Temp;
	Temp = Encoder_Count;
	Encoder_Count = 0;
	return Temp;
}

/**
  * 函    数：EXTI0外部中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */

void EXTI1_IRQHandler(void)
{

	if (EXTI_GetITStatus(EXTI_Line1) == SET)		//判断是否是外部中断0号线触发的中断
	{
		
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(ENCODER_Port, ENCODERa_PIN) == 0)
		{
			if (GPIO_ReadInputDataBit(ENCODER_Port, ENCODERb_PIN) == 0)		//PB0的下降沿触发中断，此时检测另一相PB1的电平，目的是判断旋转方向
			{

				Encoder_Count --;					//此方向定义为反转，计数变量自减

			}
		}
		EXTI_ClearITPendingBit(EXTI_Line1);			//清除外部中断0号线的中断标志位

													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}

/**
  * 函    数：EXTI1外部中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */

void EXTI9_5_IRQHandler(void)
{

	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//判断是否是外部中断1号线触发的中断
	{
		
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(ENCODER_Port, ENCODERb_PIN) == 0)
		{
			if (GPIO_ReadInputDataBit(ENCODER_Port, ENCODERa_PIN) == 0)		//PB1的下降沿触发中断，此时检测另一相PB0的电平，目的是判断旋转方向
			{

				Encoder_Count ++;					//此方向定义为正转，计数变量自增
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line5);			//清除外部中断1号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死

	}
}
