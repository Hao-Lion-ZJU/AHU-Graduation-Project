#ifndef __LED_H
#define	__LED_H

#include "stm32f1xx.h"

//引脚定义
/*******************************************************/
//R 红色灯
#define LED1_PIN                  GPIO_PIN_5                
//G 绿色灯
#define LED2_PIN                  GPIO_PIN_4 

#define LED_GPIO_PORT            GPIOB                      
#define LED_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()


/************************************************************/


/** 控制LED灯亮灭的宏，
	* LED低电平亮，设置ON=0，OFF=1
	* 若LED高电平亮，把宏设置成ON=1 ，OFF=0 即可
	*/
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	HAL_GPIO_WritePin(LED_GPIO_PORT,LED1_PIN,a)


#define LED2(a)	HAL_GPIO_WritePin(LED_GPIO_PORT,LED2_PIN,a)






/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			  //设置为高电平		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(LED_GPIO_PORT,LED1_PIN)
#define LED1_OFF		digitalHi(LED_GPIO_PORT,LED1_PIN)
#define LED1_ON			digitalLo(LED_GPIO_PORT,LED1_PIN)

#define LED2_TOGGLE		digitalToggle(LED_GPIO_PORT,LED2_PIN)
#define LED2_OFF		digitalHi(LED_GPIO_PORT,LED2_PIN)
#define LED2_ON			digitalLo(LED_GPIO_PORT,LED2_PIN)


void LED_GPIO_Config(void);

#endif /* __LED_H */
