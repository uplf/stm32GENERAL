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

	RCC_APB1PeriphClockCmd(TIMxtoRCCPeriph(TIMx), ENABLE);			//开启TIM2的时钟
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

void PWM_setMODE(TIM_TypeDef* TIMx)
{
		/*输出比较初始化*/
	TIM_OCInitTypeDef TIM_OCInitStructure;							//定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);							//结构体初始化，若结构体没有完整赋值
																	//则最好执行此函数，给结构体所有成员都赋一个默认值
																	//避免结构体初值不确定的问题
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//初始的CCR值
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);						//将结构体变量交给TIM_OC1Init，配置TIM2的输出比较通道1
}
void PWM_setIO(TIM_TypeDef* TIMx,uint16_t AF_INDEX){
	AF_INDEX=AF_INDEX&0x0F;
	PIN_setMODE(GPIOA,TimIndexToPWMPins(TIMx,AF_INDEX),GPIO_Mode_AF_PP);
	AF_INDEX=AF_INDEX|0x800;
	PIN_setMODE(GPIOB,TimIndexToPWMPins(TIMx,AF_INDEX),GPIO_Mode_AF_PP);
}

uint16_t dutyPWM_calCCR(TIM_TypeDef* TIMx,uint16_t duty)
{
	return (duty<100)?(TIMx->ARR+1)*duty/100:TIMx->ARR;
}

void PWMI_setMODE(TIM_TypeDef* TIMx,uint16_t TIM_Channel_x)
{
	/*PWMI模式初始化*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							//定义结构体变量
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_x;				//选择配置定时器通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//输入滤波器参数，可以过滤信号抖动
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//极性，选择为上升沿触发捕获
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			//捕获预分频，选择不分频，每次信号都触发捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//输入信号交叉，选择直通，不交叉
	TIM_PWMIConfig(TIMx, &TIM_ICInitStructure);						//将结构体变量交给TIM_PWMIConfig，配置TIM3的输入捕获通道
																	//此函数同时会把另一个通道配置为相反的配置，实现PWMI模式
	
		/*选择触发源及从模式*/
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);					//触发源选择TI1FP1
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Reset);					//从模式选择复位
																	//即TI1产生上升沿时，会触发CNT归零
	
}
		uint32_t PWMI_getFREQ(TIM_TypeDef* TIMx)
		{
			return 1000000 / (TIM_GetCapture1(TIMx) + 1);		//测周法得到频率fx = fc / N，这里不执行+1的操作也可
		}
		uint32_t PWMI_getDUTY(TIM_TypeDef* TIMx)
		{
				return (TIM_GetCapture4(TIMx) + 1) * 100 / (TIM_GetCapture1(TIMx) + 1);	
		}


void sADC_setMODE(uint8_t ADC_Channel_x,ADC_TypeDef* ADCx)
{
	RCC_APB2PeriphClockCmd(ADCxtoRCC_Periph(ADCx), ENABLE);	//开启ADC1的时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	
	/*规则组通道配置*/
	ADC_RegularChannelConfig(ADCx, ADC_Channel_x, 1, ADC_SampleTime_55Cycles5);		//规则组序列1的位置，配置为通道0
	
	/*ADC初始化*/
	ADC_InitTypeDef ADC_InitStructure;						//定义结构体变量
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//模式，选择独立模式，即单独使用ADC1
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据对齐，选择右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发，使用软件触发，不需要外部触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//连续转换，失能，每转换一次规则组序列后停止
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//扫描模式，失能，只转换规则组的序列1这一个位置
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//通道数，为1，仅在扫描模式下，才需要指定大于1的数，在非扫描模式下，只能是1
	ADC_Init(ADCx, &ADC_InitStructure);						//将结构体变量交给ADC_Init，配置ADC1
}

	void sADC_caliSTART(ADC_TypeDef* ADCx)
	{
			/*ADC使能*/
	ADC_Cmd(ADC1, ENABLE);									//使能ADC1，ADC开始运行
	
		/*ADC校准*/
		ADC_ResetCalibration(ADC1);								//固定流程，内部有电路会自动执行校准
		while (ADC_GetResetCalibrationStatus(ADC1) == SET);
		ADC_StartCalibration(ADC1);
		while (ADC_GetCalibrationStatus(ADC1) == SET);
	}
	uint16_t sADC_getDATA(ADC_TypeDef* ADCx)
	{
		ADC_SoftwareStartConvCmd(ADCx, ENABLE);					//软件触发AD转换一次
		while (ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);	//等待EOC标志位，即等待AD转换结束
		return ADC_GetConversionValue(ADC1);					//读数据寄存器，得到AD转换的结果
	}
	
	
	
	