#include "CONFgeneral.h"
#include "map.h"


void PIN_setMODE(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,GPIOMode_TypeDef MODE)
{
		/*开启时钟*/
	RCC_APB2PeriphClockCmd(GPIOToRCCPeriph(GPIOx), ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = MODE;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);						//将PB1和PB11引脚初始化为上拉输入
}


uint8_t PIN_readBIT(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
	    // 直接读取 GPIOx->IDR，返回引脚状态
    if ((GPIOx->IDR & GPIO_Pin_x) != 0) {
        return 1; // 高电平
    } else {
        return 0; // 低电平
    }
}
void PIN_writeBIT(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x,BitAction x)
{
	GPIO_WriteBit(GPIOx,GPIO_Pin_x,x);
}


//section2
void EXTI_setGROUP(uint32_t NVIC_PriorityGroup_x){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);	
}

int8_t gpioINT_setEXTI(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x,EXTITrigger_TypeDef EXTI_Trigger_x)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟
		/*AFIO选择中断引脚*/
	GPIO_EXTILineConfig(GPIOToPortSource(GPIOx), GPIOPinToPinSource(GPIO_Pin_x));//将外部中断的14号线映射到GPIOB，即选择PB14为外部中断引脚
		/*EXTI初始化*/
	EXTI_InitTypeDef EXTI_InitStructure;						//定义结构体变量
	EXTI_InitStructure.EXTI_Line = GPIOPinToEXTILine(GPIO_Pin_x);					//选择配置外部中断的14号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//指定外部中断线使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//指定外部中断线为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//指定外部中断线为下降沿触发
	EXTI_Init(&EXTI_InitStructure);								//将结构体变量交给EXTI_Init，配置EXTI外设
	return EXTILineToIRQn(EXTI_InitStructure.EXTI_Line);
}

int8_t timerINT_preset(TIM_TypeDef* TIMx)
{
		/*中断输出配置*/
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);						//清除定时器更新标志位
																//TIM_TimeBaseInit函数末尾，手动产生了更新事件
																//若不清除此标志位，则开启中断后，会立刻进入一次中断
																//如果不介意此问题，则不清除此标志位也可
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);					//开启TIM2的更新中断
	return TIMxtoIRQn(TIMx);
}



	
void INT_setNVIC(uint8_t NVIC_IRQChannel,uint8_t PP,uint8_t SP,FunctionalState x){
		/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;		//选择配置NVIC的EXTI15_10线
	NVIC_InitStructure.NVIC_IRQChannelCmd = x;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PP;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SP;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
}


//Section3
void TIMER_setMODE(TIM_TypeDef* TIMx,uint16_t period,uint16_t prescaler,uint16_t TIM_CounterMode_x)
{

	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_x; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = period - 1;					//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = prescaler - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元
	
}
void freqTIMER_setMODE(TIM_TypeDef* TIMx,uint16_t freq,uint16_t TIM_CounterMode_x)
{
	TIMER_setMODE( TIMx,10000,7200/freq, TIM_CounterMode_x);
}



