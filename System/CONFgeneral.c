#include "CONFgeneral.h"
#include "map.h"


void PIN_setMODE(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,GPIOMode_TypeDef MODE)
{
		/*����ʱ��*/
	RCC_APB2PeriphClockCmd(GPIOToRCCPeriph(GPIOx), ENABLE);		//����GPIOB��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = MODE;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);						//��PB1��PB11���ų�ʼ��Ϊ��������
}


uint8_t PIN_readBIT(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
	    // ֱ�Ӷ�ȡ GPIOx->IDR����������״̬
    if ((GPIOx->IDR & GPIO_Pin_x) != 0) {
        return 1; // �ߵ�ƽ
    } else {
        return 0; // �͵�ƽ
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//����AFIO��ʱ�ӣ��ⲿ�жϱ��뿪��AFIO��ʱ��
		/*AFIOѡ���ж�����*/
	GPIO_EXTILineConfig(GPIOToPortSource(GPIOx), GPIOPinToPinSource(GPIO_Pin_x));//���ⲿ�жϵ�14����ӳ�䵽GPIOB����ѡ��PB14Ϊ�ⲿ�ж�����
		/*EXTI��ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;						//����ṹ�����
	EXTI_InitStructure.EXTI_Line = GPIOPinToEXTILine(GPIO_Pin_x);					//ѡ�������ⲿ�жϵ�14����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//ָ���ⲿ�ж���ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//ָ���ⲿ�ж���Ϊ�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//ָ���ⲿ�ж���Ϊ�½��ش���
	EXTI_Init(&EXTI_InitStructure);								//���ṹ���������EXTI_Init������EXTI����
	return EXTILineToIRQn(EXTI_InitStructure.EXTI_Line);
}

int8_t timerINT_preset(TIM_TypeDef* TIMx)
{
		/*�ж��������*/
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);						//�����ʱ�����±�־λ
																//TIM_TimeBaseInit����ĩβ���ֶ������˸����¼�
																//��������˱�־λ�������жϺ󣬻����̽���һ���ж�
																//�������������⣬������˱�־λҲ��
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);					//����TIM2�ĸ����ж�
	return TIMxtoIRQn(TIMx);
}



	
void INT_setNVIC(uint8_t NVIC_IRQChannel,uint8_t PP,uint8_t SP,FunctionalState x){
		/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;		//ѡ������NVIC��EXTI15_10��
	NVIC_InitStructure.NVIC_IRQChannelCmd = x;				//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PP;	//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SP;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����
}


//Section3
void TIMER_setMODE(TIM_TypeDef* TIMx,uint16_t period,uint16_t prescaler,uint16_t TIM_CounterMode_x)
{

	RCC_APB1PeriphClockCmd(TIMxtoRCCPeriph(TIMx), ENABLE);			//����TIM2��ʱ��
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_x; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = period - 1;					//�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = prescaler - 1;				//Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM2��ʱ����Ԫ
	
}
void freqTIMER_setMODE(TIM_TypeDef* TIMx,uint16_t freq,uint16_t TIM_CounterMode_x)
{
	TIMER_setMODE( TIMx,10000,7200/freq, TIM_CounterMode_x);
}

void PWM_setMODE(TIM_TypeDef* TIMx)
{
		/*����Ƚϳ�ʼ��*/
	TIM_OCInitTypeDef TIM_OCInitStructure;							//����ṹ�����
	TIM_OCStructInit(&TIM_OCInitStructure);							//�ṹ���ʼ�������ṹ��û��������ֵ
																	//�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
																	//����ṹ���ֵ��ȷ��������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//����Ƚ�ģʽ��ѡ��PWMģʽ1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//������ԣ�ѡ��Ϊ�ߣ���ѡ����Ϊ�ͣ�������ߵ͵�ƽȡ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//���ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;								//��ʼ��CCRֵ
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);						//���ṹ���������TIM_OC1Init������TIM2������Ƚ�ͨ��1
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
	/*PWMIģʽ��ʼ��*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							//����ṹ�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_x;				//ѡ�����ö�ʱ��ͨ��1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//�����˲������������Թ����źŶ���
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//���ԣ�ѡ��Ϊ�����ش�������
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			//����Ԥ��Ƶ��ѡ�񲻷�Ƶ��ÿ���źŶ���������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//�����źŽ��棬ѡ��ֱͨ��������
	TIM_PWMIConfig(TIMx, &TIM_ICInitStructure);						//���ṹ���������TIM_PWMIConfig������TIM3�����벶��ͨ��
																	//�˺���ͬʱ�����һ��ͨ������Ϊ�෴�����ã�ʵ��PWMIģʽ
	
		/*ѡ�񴥷�Դ����ģʽ*/
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);					//����Դѡ��TI1FP1
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Reset);					//��ģʽѡ��λ
																	//��TI1����������ʱ���ᴥ��CNT����
	
}
		uint32_t PWMI_getFREQ(TIM_TypeDef* TIMx)
		{
			return 1000000 / (TIM_GetCapture1(TIMx) + 1);		//���ܷ��õ�Ƶ��fx = fc / N�����ﲻִ��+1�Ĳ���Ҳ��
		}
		uint32_t PWMI_getDUTY(TIM_TypeDef* TIMx)
		{
				return (TIM_GetCapture4(TIMx) + 1) * 100 / (TIM_GetCapture1(TIMx) + 1);	
		}


void sADC_setMODE(uint8_t ADC_Channel_x,ADC_TypeDef* ADCx)
{
	RCC_APB2PeriphClockCmd(ADCxtoRCC_Periph(ADCx), ENABLE);	//����ADC1��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	
	/*������ͨ������*/
	ADC_RegularChannelConfig(ADCx, ADC_Channel_x, 1, ADC_SampleTime_55Cycles5);		//����������1��λ�ã�����Ϊͨ��0
	
	/*ADC��ʼ��*/
	ADC_InitTypeDef ADC_InitStructure;						//����ṹ�����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//ģʽ��ѡ�����ģʽ��������ʹ��ADC1
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//���ݶ��룬ѡ���Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ������ʹ���������������Ҫ�ⲿ����
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//����ת����ʧ�ܣ�ÿת��һ�ι��������к�ֹͣ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//ɨ��ģʽ��ʧ�ܣ�ֻת�������������1��һ��λ��
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//ͨ������Ϊ1������ɨ��ģʽ�£�����Ҫָ������1�������ڷ�ɨ��ģʽ�£�ֻ����1
	ADC_Init(ADCx, &ADC_InitStructure);						//���ṹ���������ADC_Init������ADC1
}

	void sADC_caliSTART(ADC_TypeDef* ADCx)
	{
			/*ADCʹ��*/
	ADC_Cmd(ADC1, ENABLE);									//ʹ��ADC1��ADC��ʼ����
	
		/*ADCУ׼*/
		ADC_ResetCalibration(ADC1);								//�̶����̣��ڲ��е�·���Զ�ִ��У׼
		while (ADC_GetResetCalibrationStatus(ADC1) == SET);
		ADC_StartCalibration(ADC1);
		while (ADC_GetCalibrationStatus(ADC1) == SET);
	}
	uint16_t sADC_getDATA(ADC_TypeDef* ADCx)
	{
		ADC_SoftwareStartConvCmd(ADCx, ENABLE);					//�������ADת��һ��
		while (ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);	//�ȴ�EOC��־λ�����ȴ�ADת������
		return ADC_GetConversionValue(ADC1);					//�����ݼĴ������õ�ADת���Ľ��
	}
	
	
	
	