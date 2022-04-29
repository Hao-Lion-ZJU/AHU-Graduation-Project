#ifndef _DELAY_H
#define _DELAY_H
#include "main.h"
#define u8 uint8_t
#define u16 uint16_t

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void TimingDelay_Decrement(void);
extern void Delay_ns (u8 t);			// ns延时
extern void Delay_1us (u8 t);							// 延时时基:1us
extern void Delay_2us (u16 t);							// 延时时基:2us
extern void Delay_10us (u8 t);   					// 延时时基:10us
extern void Delay_882us (void);							// 延时822us
extern void Delay_250us (u8 t);  // 延时时基:250us
extern void Delay_5ms (u8 t);							// 延时时基:5ms
extern void Delay_1ms (u8 t);		// 延时时基:1ms
extern void Delay_50ms (u8 t);		// 延时时基:50ms
extern void Delay(__IO uint32_t nCount);

#endif

//-----------------------------------------------------------------
// End Of File
