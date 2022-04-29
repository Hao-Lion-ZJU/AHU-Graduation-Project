/**
  ****************************(C) COPYRIGHT 2022 AHU****************************
  * @file       micostep.c/h
  * @brief      控制步进电机底层
  * @note       
  * @history
  *  Version    Date            Author          Liangliang Hao (郝亮亮)
	* E-mail  1526885155@qq.com
  *  V1.0.0     April-4-2022     AHU            1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 AHU****************************
  */
#include "micostep.h"
#define p 3.0					//丝杆导程为3mm
#define ppr			3200		//电机每转一圈所需的脉冲数

 step_ctrl SMD1={0};
 step_ctrl SMD2={0};

/**
  * @brief          1号步进电机使能函数
  * @param[in]      DISABEL：不输出
  * @param[in]      ENABLE：输出
  * @retval         none
  */
void SMD1_ENA(FunctionalState NewState)
{
    if(NewState)
    {
      //ENA失能，禁止驱动器输出
			HAL_GPIO_WritePin(SMD1_ENA_PORT,SMD1_ENA_PIN,1);
    }
    else
    {
      //ENA使能  
			HAL_GPIO_WritePin(SMD1_ENA_PORT,SMD1_ENA_PIN,0);     
    }  
}

/**
  * @brief          2号步进电机使能函数
  * @param[in]      DISABEL：不输出
  * @param[in]      ENABLE：输出
  * @retval         none
  */
void SMD2_ENA(FunctionalState NewState)
{
    if(NewState)
    {
      //ENA失能，禁止驱动器输出
			HAL_GPIO_WritePin(SMD2_ENA_PORT,SMD2_ENA_PIN,1);
    }
    else
    {
      //ENA使能  
			HAL_GPIO_WritePin(SMD2_ENA_PORT,SMD2_ENA_PIN,0);     
    }  
}


/**
  * @brief          使控制器输出若干脉冲
	* @param[in]      SMD:传进的步进电机参数句柄
	* @param[in]      steps:输出的脉冲数
	* @param[in]      dir:旋转方向
  * @retval         none
  */
void SMD_Move(step_ctrl* SMD,unsigned int steps,unsigned char dir)
{
	SMD->dir=dir;
	SMD->cur_count=0; //重新开始计数
	SMD->max_count=steps;
	SMD->run_state=running;
	if(SMD==&SMD1)
	{
		SMD1_DIR(dir);
		HAL_Delay(10);
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);               //开启pwm
	}
	else
	{
		SMD2_DIR(dir);
		HAL_Delay(10);
		HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_4);               //开启pwm
	}

}


/**
  * @brief          XY平台移动函数
	* @param[in]      DirStatus:  0:前   1：后   2：左		3:右
	* @param[in]      dis:  移动距离  单位：mm
  * @retval         none
  */
void Plane_Move(DirStatus DIR,unsigned int dis)
{
	unsigned int steps=0;
	steps=(dis/3.0)*ppr;				//脉冲数=移动距离/丝杆导程*每圈脉冲数
	if(DIR==Front)
	{
		SMD_Move(&SMD2,steps,CW);
	}
	else if(DIR==Back)
	{
		SMD_Move(&SMD2,steps,CCW);
	}
	else if(DIR==Left)
	{
		SMD_Move(&SMD1,steps,CW);
	}
	else if(DIR==Right)
	{
		SMD_Move(&SMD1,steps,CCW);
	}
}

/**
  * @brief          返回当前平台运行状态
	* @param[in]      none
	* @retval         Statues:    0:正在运行		1：停止状态
  */
unsigned char Get_PlaneStatus(void)
{
	if((SMD1.run_state==stop)&&(SMD2.run_state==stop))
	{
		return 1;
	}
	else
		return 0;
}



/**
  * @brief          PWM输出一个脉冲的回调函数
	* @param[in]      htim:定时器句柄  htim3控制一号步进电机，htim2控制二号步进电机
  * @retval         none
  */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
   */
	if(htim==(&htim3))
	{
		if(SMD1.cur_count<SMD1.max_count)	//当脉冲累计到应输出值时,停止PWM输出
		{
			SMD1.cur_count++;
		}
		else
		{
			SMD1.cur_count=0;
			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);
			SMD1.run_state=stop;
		}
	}
	if(htim==(&htim2))
	{
		if(SMD2.cur_count<SMD2.max_count)
		{
			SMD2.cur_count++;
		}
		else
		{
			SMD2.cur_count=0;
			HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_4);
			SMD2.run_state=stop;
		}
	}
	if((SMD1.run_state==stop)&&(SMD2.run_state==stop))			//如果两个电机均转动完毕，则
	{
		unsigned char ch='y';
		HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
	}
}

