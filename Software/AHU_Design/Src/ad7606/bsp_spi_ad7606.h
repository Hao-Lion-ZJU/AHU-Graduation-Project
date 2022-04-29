#ifndef __BSP_AD7606_H
#define __BSP_AD7606_H

#include "stm32f1xx.h"
#include <stdio.h>



/* 每个样本2字节，采集通道 */
#define CH_NUM			8				/* 采集8通道 */
#define SIZE		8*2		
#define COUNT		10	//每个点的采样次数

typedef enum 
{
	AD1=0,
	AD2=1
}AD_NUM;


/* 定义AD7606的SPI GPIO */
/* 配置AD1的SPI1 */
#define AD_SPI1_SCK_PIN                   GPIO_PIN_5				
#define AD_SPI1_SCK_GPIO_PORT             GPIOA						

#define AD_SPI1_MISO_PIN                  GPIO_PIN_6				
#define AD_SPI1_MISO_GPIO_PORT            GPIOA		

#define AD_SPI1_MOSI_PIN                  GPIO_PIN_7				
#define AD_SPI1_MOSI_GPIO_PORT            GPIOA

#define AD_SPI1_CS_PIN                    GPIO_PIN_13
#define AD_SPI1_CS_GPIO_PORT              GPIOE				


/*　定义AD1其它的GPIO */
#define AD1_RESET_PIN                     GPIO_PIN_12
#define AD1_RESET_GPIO_PORT               GPIOE				

#define AD1_CONVST_PIN                    GPIO_PIN_11
#define AD1_CONVST_GPIO_PORT              GPIOE		

#define AD1_RANGE_PIN                     GPIO_PIN_10
#define AD1_RAGEE_GPIO_PORT               GPIOE		

#define AD1_OSC0_PIN                     	GPIO_PIN_7
#define AD1_OSC0_GPIO_PORT               	GPIOD		

#define AD1_OSC1_PIN                     	GPIO_PIN_8
#define AD1_OSC1_GPIO_PORT               	GPIOD		

#define AD1_OSC2_PIN                     	GPIO_PIN_9
#define AD1_OSC2_GPIO_PORT               	GPIOD

#define AD1_BUSY_PIN 											GPIO_PIN_14
#define AD1_BUSY_GPIO_Port 								GPIOE


/* 配置AD2的SPI2 */
#define AD_SPI2_SCK_PIN                   GPIO_PIN_13				
#define AD_SPI2_SCK_GPIO_PORT             GPIOB						

#define AD_SPI2_MISO_PIN                  GPIO_PIN_14				
#define AD_SPI2_MISO_GPIO_PORT            GPIOB	

#define AD_SPI2_MOSI_PIN                  GPIO_PIN_15				
#define AD_SPI2_MOSI_GPIO_PORT            GPIOB	

#define AD_SPI2_CS_PIN                    GPIO_PIN_11
#define AD_SPI2_CS_GPIO_PORT              GPIOB				


/*　定义AD2其它的GPIO */
#define AD2_RESET_PIN                     GPIO_PIN_10
#define AD2_RESET_GPIO_PORT               GPIOB				

#define AD2_CONVST_PIN                    GPIO_PIN_9
#define AD2_CONVST_GPIO_PORT              GPIOB		

#define AD2_RANGE_PIN                     GPIO_PIN_0
#define AD2_RAGEE_GPIO_PORT               GPIOB		

#define AD2_OSC0_PIN                     	GPIO_PIN_9
#define AD2_OSC0_GPIO_PORT               	GPIOE		

#define AD2_OSC1_PIN                     	GPIO_PIN_8
#define AD2_OSC1_GPIO_PORT               	GPIOE		

#define AD2_OSC2_PIN                     	GPIO_PIN_7
#define AD2_OSC2_GPIO_PORT               	GPIOE

#define AD2_BUSY_PIN 											GPIO_PIN_12
#define AD2_BUSY_GPIO_Port 								GPIOB




/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			  //设置为高电平		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}				//输出低电平

/*AD1控制GPIO宏定义*/
#define AD1_SCK_LOW()     				digitalLo(AD_SPI1_SCK_GPIO_PORT,AD_SPI1_SCK_PIN)
#define AD1_SCK_HIGH()     			digitalHi(AD_SPI1_SCK_GPIO_PORT,AD_SPI1_SCK_PIN)

#define AD1_CS_LOW()     				digitalLo(AD_SPI1_CS_GPIO_PORT,AD_SPI1_CS_PIN)
#define AD1_CS_HIGH()     			digitalHi(AD_SPI1_CS_GPIO_PORT,AD_SPI1_CS_PIN)

#define AD1_RESET_LOW()					digitalLo(AD1_RESET_GPIO_PORT,AD1_RESET_PIN)
#define AD1_RESET_HIGH()				digitalHi(AD1_RESET_GPIO_PORT,AD1_RESET_PIN)
	
#define AD1_CONVST_LOW()				digitalLo(AD1_CONVST_GPIO_PORT,AD1_CONVST_PIN)
#define AD1_CONVST_HIGH()				digitalHi(AD1_CONVST_GPIO_PORT,AD1_CONVST_PIN)

#define AD1_RANGE_5V()					digitalLo(AD1_RAGEE_GPIO_PORT,AD1_RANGE_PIN)
#define AD1_RANGE_10V()					digitalHi(AD1_RAGEE_GPIO_PORT,AD1_RANGE_PIN)

#define AD1_OS0_0()							digitalLo(AD1_OSC0_GPIO_PORT,AD1_OSC0_PIN)
#define AD1_OS0_1()							digitalHi(AD1_OSC0_GPIO_PORT,AD1_OSC0_PIN)

#define AD1_OS1_0()							digitalLo(AD1_OSC1_GPIO_PORT,AD1_OSC1_PIN)
#define AD1_OS1_1()							digitalHi(AD1_OSC1_GPIO_PORT,AD1_OSC1_PIN)

#define AD1_OS2_0()							digitalLo(AD1_OSC2_GPIO_PORT,AD1_OSC2_PIN)
#define AD1_OS2_1()							digitalHi(AD1_OSC2_GPIO_PORT,AD1_OSC2_PIN)

#define AD1_Read_Dat			HAL_GPIO_ReadPin(AD_SPI1_MISO_GPIO_PORT,AD_SPI1_MISO_PIN)//读取数据引脚状态
#define AD1_Read_Busy			HAL_GPIO_ReadPin(AD1_BUSY_GPIO_Port,AD1_BUSY_PIN)//读取数据引脚状态


/*AD2控制GPIO宏定义*/

#define AD2_SCK_LOW()     				digitalLo(AD_SPI2_SCK_GPIO_PORT,AD_SPI2_SCK_PIN)
#define AD2_SCK_HIGH()     			digitalHi(AD_SPI2_SCK_GPIO_PORT,AD_SPI2_SCK_PIN)

#define AD2_CS_LOW()     				digitalLo(AD_SPI2_CS_GPIO_PORT,AD_SPI2_CS_PIN)
#define AD2_CS_HIGH()     			digitalHi(AD_SPI2_CS_GPIO_PORT,AD_SPI2_CS_PIN)

#define AD2_RESET_LOW()					digitalLo(AD2_RESET_GPIO_PORT,AD2_RESET_PIN)
#define AD2_RESET_HIGH()				digitalHi(AD2_RESET_GPIO_PORT,AD2_RESET_PIN)
	
#define AD2_CONVST_LOW()				digitalLo(AD2_CONVST_GPIO_PORT,AD2_CONVST_PIN)
#define AD2_CONVST_HIGH()				digitalHi(AD2_CONVST_GPIO_PORT,AD2_CONVST_PIN)

#define AD2_RANGE_5V()					digitalLo(AD2_RAGEE_GPIO_PORT,AD2_RANGE_PIN)
#define AD2_RANGE_10V()					digitalHi(AD2_RAGEE_GPIO_PORT,AD2_RANGE_PIN)

#define AD2_OS0_0()							digitalLo(AD2_OSC0_GPIO_PORT,AD2_OSC0_PIN)
#define AD2_OS0_1()							digitalHi(AD2_OSC0_GPIO_PORT,AD2_OSC0_PIN)

#define AD2_OS1_0()							digitalLo(AD2_OSC1_GPIO_PORT,AD2_OSC1_PIN)
#define AD2_OS1_1()							digitalHi(AD2_OSC1_GPIO_PORT,AD2_OSC1_PIN)

#define AD2_OS2_0()							digitalLo(AD2_OSC2_GPIO_PORT,AD2_OSC2_PIN)
#define AD2_OS2_1()							digitalHi(AD2_OSC2_GPIO_PORT,AD2_OSC2_PIN)

#define AD2_Read_Dat			HAL_GPIO_ReadPin(AD_SPI2_MISO_GPIO_PORT,AD_SPI2_MISO_PIN)//读取数据引脚状态
#define AD2_Read_Busy			HAL_GPIO_ReadPin(AD2_BUSY_GPIO_Port,AD2_BUSY_PIN)//读取数据引脚状态



/* 供外部调用的函数声明 */
void ad7606_Reset(AD_NUM ad_num);
void ad7606_SetOS(uint8_t _ucMode,AD_NUM ad_num);
void AD7606_Config(void);
void ad7606_StartRecord(uint32_t _ulFreq);
void ad7606_StopRecord(void);
void ad7606_IRQSrc(void);
void ad7606_Init(void);

extern  uint16_t g_tAD[SIZE];

#endif
/*********************************************END OF FILE**********************/
