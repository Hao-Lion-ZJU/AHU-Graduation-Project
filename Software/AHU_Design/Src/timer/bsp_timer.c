#include"bsp_timer.h"

TIM_HandleTypeDef TIM_TimeBaseStructure;


/**
*********************************************************************************************************
* @brief  时钟与中断配置
* @param  无
* @retval 无
*********************************************************************************************************
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{  
    /* 基本定时器外设时钟使能 */
    __HAL_RCC_TIM4_CLK_ENABLE();

    /* 外设中断配置 */
    HAL_NVIC_SetPriority(TIM4_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);  
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SET_TIM4_FREQ
*	功能说明: 设置TIM4定时器频率
*	形    参：_ulFreq : 采样频率，单位Hz，
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SET_TIM4_FREQ(uint32_t _ulFreq)
{
	uint16_t usPrescaler;
	uint16_t usPeriod;

//	HAL_TIM_Base_DeInit(&TIM_TimeBaseStructure);	/* 复位TIM4定时器 */

	/* TIM4 configuration 
		TIM4CLK = 72 MHz	
	*/
	if (_ulFreq == 0)
	{
		return;		/* 采样频率为0，停止采样 */
	}
	else if (_ulFreq <= 100)   /* 采样频率小于100Hz */
	{
		usPrescaler = 36000;		/* TM2CLK = 72 000 000/36000 = 2000 */
		usPeriod = 2000 / _ulFreq;			 	
	}
	else if (_ulFreq <= 200000)	/* 采样频率 ：100Hz - 200kHz */
	{
		usPrescaler = 36 - 1;		/* TM2CLK = 36 000 000/36 = 2 000 000 */
		usPeriod = 2000000 / _ulFreq;
	}	
	else	/* 采样频率大于 200kHz */
	{
		return;
	}
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  TIM_TimeBaseStructure.Instance = TIM4;
	// 时钟预分频数
	TIM_TimeBaseStructure.Init.Prescaler = usPrescaler-1;
	// 计数器计数模式，设置为向上计数
  TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
  TIM_TimeBaseStructure.Init.Period = usPeriod-1;
	// 时钟分频因子 ，没用到不用管
  TIM_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	// 重复计数器的值，没用到不用管
  TIM_TimeBaseStructure.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&TIM_TimeBaseStructure);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&TIM_TimeBaseStructure, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&TIM_TimeBaseStructure, &sMasterConfig);
	
	/* 在中断模式下启动定时器 */
  HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure);
}  

/*********************************************END OF FILE**********************/
