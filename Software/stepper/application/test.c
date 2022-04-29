/**
  ****************************(C) COPYRIGHT 2022 AHU****************************
  * @file       test.c/h
  * @brief      串口控制移动平台
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
#include "test.h"
#include <stdlib.h>


unsigned char uart_cmd;

/**
  * @brief  处理串口接收到的数据
  * @paramin  无
  * @retval 无
  */
void DealSerialData(void)
{
	unsigned int dis=0;
	//接收到' '开始解析命令
	if(uart_cmd==TRUE)
	{
		if(UART_RxBuffer[0] == 'f')		//向前移动
      {
				//从串口获取步数
        dis = atoi((char const *)UART_RxBuffer+1);
				Plane_Move(0,dis);
			}
			else if(UART_RxBuffer[0] == 'b')		//向后移动
			{
				dis = atoi((char const *)UART_RxBuffer+1);
				Plane_Move(1,dis);
			}
			else if(UART_RxBuffer[0] == 'l')		//向左移动
			{
				dis = atoi((char const *)UART_RxBuffer+1);
				Plane_Move(2,dis);
			}
			else if(UART_RxBuffer[0] == 'r')		//向右移动
			{
				dis = atoi((char const *)UART_RxBuffer+1);
				Plane_Move(3,dis);
			}
		uart_cmd = FALSE;
		uart_FlushRxBuffer();
	}
}