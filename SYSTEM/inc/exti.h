#ifndef _EXTI_H
#define _EXTI_H

#include "sys.h"
#include "ad7606.h"
#include "timer.h"

extern u8 calc_flag;

void EXTIX_Init(void);	//外部中断初始化

#endif
