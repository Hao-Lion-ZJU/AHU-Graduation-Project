#include "bsp_spi_ad7606.h"
#include "./timer/bsp_timer.h"
#include "./led/bsp_led.h" 
#include "./usart/bsp_debug_usart.h"
#include "Delay.h"



uint16_t g_tAD[SIZE]={0};	/* 定义一个交换缓冲区，用于存储AD采集数据，并用于串口发送 */

 /**
  * @brief  初始化控制两块AD7606芯片的引脚
  * @param  无
  * @retval 无
  */
void AD7606_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 

   __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOE_CLK_ENABLE();
   __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = AD_SPI1_SCK_PIN|AD_SPI1_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD_SPI1_SCK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AD_SPI1_MISO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AD_SPI1_MISO_GPIO_PORT, &GPIO_InitStruct);
	 
	  GPIO_InitStruct.Pin = AD_SPI1_CS_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(AD_SPI1_CS_GPIO_PORT, &GPIO_InitStruct);
		
		
		/*配置AD1其他控制GPIO*/
		GPIO_InitStruct.Pin = AD1_BUSY_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(AD1_BUSY_GPIO_Port, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = AD1_RANGE_PIN|AD1_CONVST_PIN|AD1_RESET_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = AD1_OSC2_PIN|AD1_OSC1_PIN|AD1_OSC0_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	


	 /**SPI2 GPIO Configuration 
		PB11		 ------> SPI2_CS
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO 
    */
    GPIO_InitStruct.Pin = AD_SPI2_SCK_PIN|AD_SPI2_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD_SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AD_SPI2_MISO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AD_SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);
	 
	  GPIO_InitStruct.Pin = AD_SPI2_CS_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(AD_SPI2_CS_GPIO_PORT, &GPIO_InitStruct);
		
		
		/*配置AD2其他控制GPIO*/
		GPIO_InitStruct.Pin = AD2_BUSY_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		//GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(AD1_BUSY_GPIO_Port, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = AD2_CONVST_PIN|AD2_RESET_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = AD2_OSC2_PIN|AD2_OSC1_PIN|AD2_OSC0_PIN|AD2_RANGE_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
				
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_StartConv
*	功能说明: 启动AD7606的ADC转换
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_StartConv(AD_NUM ad_num)
{
	if(ad_num==AD1)
	{
		/* 上升沿开始转换，低电平持续时间至少25ns  */
		AD1_CONVST_LOW();
		Delay_ns (100);
		AD1_CONVST_HIGH();
	}
	else
	{
		/* 上升沿开始转换，低电平持续时间至少25ns  */
		AD2_CONVST_LOW();
		Delay_ns (100);
		AD2_CONVST_HIGH();
	}
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_StartRecord
*	功能说明: 开始采集
*	形    参：_ulFreq : 采样频率, 单位 HZ
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_StartRecord(uint32_t _ulFreq)
{
	//ad7606_Reset();	/* 复位硬件 */

	ad7606_StartConv(AD1);				/* 启动采样，避免第1组数据全0的问题 */
	ad7606_StartConv(AD2);			

	bsp_SET_TIM4_FREQ(_ulFreq);		/* 设置采样频率, 并使能TIM2定时采样中断 */
}



/*
*********************************************************************************************************
*	函 数 名: ad7606_SetOS
*	功能说明: 设置过采样模式（数字滤波，硬件求平均值)
*	形    参：_ucMode : 0-6  0表示无过采样，1表示2倍，2表示4倍，3表示8倍，4表示16倍
*				5表示32倍，6表示64倍
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_SetOS(uint8_t _ucMode,AD_NUM ad_num)
{
	if (_ucMode == 1)
	{
		if(ad_num==AD1)
		{
			AD1_OS2_0();
			AD1_OS1_0();
			AD1_OS0_1();
		}
		else
		{
			AD2_OS2_0();
			AD2_OS1_0();
			AD2_OS0_1();
		}
	}
	else if (_ucMode == 2)
	{
		if(ad_num==AD1)
		{
			AD1_OS2_0();
			AD1_OS1_1();
			AD1_OS0_0();
		}
		else
		{
			AD2_OS2_0();
			AD2_OS1_1();
			AD2_OS0_0();
		}
	}
	else if (_ucMode == 3)
	{
		if(ad_num==AD1)
		{
			AD1_OS2_0();
			AD1_OS1_1();
			AD1_OS0_1();
		}
		else
		{
			AD2_OS2_0();
			AD2_OS1_1();
			AD2_OS0_1();
		}
	}
	else if (_ucMode == 4)
	{
		if(ad_num==AD1)
		{
			AD1_OS2_1();
			AD1_OS1_0();
			AD1_OS0_0();
		}
		else
		{
			AD2_OS2_1();
			AD2_OS1_1();
			AD2_OS0_0();
		}
	}
	else if (_ucMode == 5)
	{
		if(ad_num==AD1)
		{
			AD1_OS2_1();
			AD1_OS1_0();
			AD1_OS0_1();
		}
		else
		{
			AD2_OS2_1();
			AD2_OS1_0();
			AD2_OS0_1();
		}
	}
	else if (_ucMode == 6)
	{
		if(ad_num==AD1)
		{
			AD1_OS2_1();
			AD1_OS1_1();
			AD1_OS0_0();
		}
		else
		{
			AD2_OS2_1();
			AD2_OS1_1();
			AD2_OS0_0();
		}
	}
	else	/* 按0处理 */
	{
		if(ad_num==AD1)
		{
			AD1_OS2_0();
			AD1_OS1_0();
			AD1_OS0_0();
		}
		else
		{
			AD2_OS2_0();
			AD2_OS1_0();
			AD2_OS0_0();
		}
	}
}


/*
*********************************************************************************************************
*	函 数 名: ad7606_Reset
*	功能说明: 复位AD7606
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_Reset(AD_NUM ad_num)
{
	if(ad_num==AD1)
	{
		/* AD7606是高电平复位，要求最小脉宽50ns */
	
		AD1_RESET_LOW();
		Delay_ns (10);
		
		AD1_RESET_HIGH();
		Delay_1us(1);
		
		AD1_RESET_LOW();
	}
	else
	{
			/* AD7606是高电平复位，要求最小脉宽50ns */
	
		AD2_RESET_LOW();
		Delay_ns (10);
		
		AD2_RESET_HIGH();
		Delay_1us(1);
		
		AD2_RESET_LOW();
	}
	
}


/*
*********************************************************************************************************
*	函 数 名: ad7606_ReadBytes
*	功能说明: 读取spi的16位采样结果
*	形    参：
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_ReadBytes(AD_NUM ad_num)
{
	u8 i,j; 
	if(ad_num==AD1)
	{
		AD1_CS_LOW();
		for(i=0;i<8;i++)  
		{
			uint16_t usData = 0;
			Delay_ns(20);	
			for(j=0;j<16;j++)
			{		
				AD1_SCK_LOW();
				Delay_ns(25);			
				usData = ((u16)(AD1_Read_Dat)<< (15-j)) + usData ;
				AD1_SCK_HIGH();
				Delay_ns(35);				
			}		
				
			g_tAD[i] = (u16)usData;
		}
		AD1_CS_HIGH();
	}
	else
	{
		//读取AD2采集结果
		AD2_CS_LOW();
		for(i=15;i>7;i--)  
		{
			uint16_t usData = 0;
			Delay_ns(20);	
			for(j=0;j<16;j++)
			{		
				AD2_SCK_LOW();
				Delay_ns(25);			
				usData = ((u16)(AD2_Read_Dat)<< (15-j)) + usData ;
				AD2_SCK_HIGH();
				Delay_ns(35);				
			}		
				
			g_tAD[i] = (u16)usData;
		}
		AD2_CS_HIGH();
	}
	
}


/*
*********************************************************************************************************
*	函 数 名: ad7606_StopRecord
*	功能说明: 停止采集
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_StopRecord(void)
{
	//若ad7606正在转换，先等待转换完成，即BUSY输出低电平
	while(HAL_GPIO_ReadPin(AD1_BUSY_GPIO_Port,AD1_BUSY_PIN)||HAL_GPIO_ReadPin(AD2_BUSY_GPIO_Port,AD2_BUSY_PIN));
	/* 关闭定时中断*/
	HAL_NVIC_DisableIRQ(TIM4_IRQn);
	/*复位两块ad7606*/
	ad7606_Reset(AD1);
	ad7606_Reset(AD2);
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_IRQSrc
*	功能说明: 定时调用本函数，用于读取AD转换器数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_IRQSrc(void)
{
	uint8_t temp;
	ad7606_StartConv(AD1);
	Delay_ns(1);
	AD1_Read_Busy;
	while(temp)
		{
			Delay_ns(10);
			temp = AD1_Read_Busy;		// 读取 BUSY的状态 	 
		}
	ad7606_ReadBytes(AD1);
		
		//AD2
	ad7606_StartConv(AD2);
	Delay_ns(1);
	AD2_Read_Busy;
	while(temp)
		{
			Delay_ns(10);
			temp = AD2_Read_Busy;		// 读取 BUSY的状态 	 
		}
	ad7606_ReadBytes(AD2);
	
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_Init
*	功能说明: 初始化ad7606
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_Init()
{
	AD1_CONVST_HIGH();
	Delay_1ms(1);
	AD1_SCK_HIGH();
	AD1_CS_HIGH();
	ad7606_SetOS(3,AD1);
	AD1_RANGE_5V();
	ad7606_Reset(AD1);				/* 硬件复位复AD7606 */
	Delay_1ms(1);
	ad7606_StartConv(AD1);
	
	
	AD2_CONVST_HIGH();
	Delay_1ms(1);
	AD2_SCK_HIGH();
	AD2_CS_HIGH();
	ad7606_SetOS(3,AD2);
	AD2_RANGE_5V();
	
	ad7606_Reset(AD2);				/* 硬件复位复AD7606 */
	Delay_1ms(1);
	ad7606_StartConv(AD2);
}


/*********************************************END OF FILE**********************/
