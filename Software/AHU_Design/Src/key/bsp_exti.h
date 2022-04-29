#ifndef __EXTI_H
#define	__EXTI_H

#include "stm32f1xx.h"

//Òý½Å¶¨Òå
/*******************************************************/
#define KEY_INT_GPIO_PORT                GPIOA
#define KEY_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE();
#define KEY_INT_GPIO_PIN                 GPIO_PIN_0
#define KEY_INT_EXTI_IRQ                 EXTI0_IRQn
#define KEY_IRQHandler                   EXTI0_IRQHandler

/*******************************************************/


void EXTI_Key_Config(void);

#endif /* __EXTI_H */

