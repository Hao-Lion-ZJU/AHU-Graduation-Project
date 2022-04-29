#ifndef __TIMER_H
#define	__TIMER_H

#include "stm32f1xx.h"


extern TIM_HandleTypeDef TIM_TimeBaseStructure;

void bsp_SET_TIM4_FREQ(uint32_t _ulFreq);

#endif /* __TIMER_H */

/*********************************************END OF FILE**********************/
