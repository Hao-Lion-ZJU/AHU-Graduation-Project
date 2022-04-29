/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

//KEY1引脚 PA0
#define            KEY1_PORT                     GPIOA
#define            KEY1_PIN                      GPIO_PIN_0

//KEY1引脚 PA0
#define            KEY2_PORT                     GPIOC
#define            KEY2_PIN                      GPIO_PIN_13

// X方向步进电机方向控制
#define            SMD1_DIR_PORT                     GPIOB
#define            SMD1_DIR_PIN                      GPIO_PIN_14

// X方向步进电机输出使能引脚
#define            SMD1_ENA_PORT                     GPIOB
#define            SMD1_ENA_PIN                      GPIO_PIN_12

// Y方向步进电机方向控制
#define            SMD2_DIR_PORT                     GPIOE
#define            SMD2_DIR_PIN                      GPIO_PIN_5

// Y方向步进电机输出使能引脚
#define            SMD2_ENA_PORT                     GPIOE
#define            SMD2_ENA_PIN                      GPIO_PIN_6

//指示灯
//红灯引脚 PB5
#define            RED_PORT                     GPIOB
#define            RED_PIN                      GPIO_PIN_5

//绿灯引脚 PB0
#define            GREEN_PORT                     GPIOB
#define            GREEN_PIN                      GPIO_PIN_0

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
