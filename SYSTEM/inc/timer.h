#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"	
#include "adc.h"
#include "fir.h"
#include "floatfft.h"
#include "ad7606.h"
#include "lcd.h"
#include "key.h"

#define Total_Ndot Ndot+FIR_ORDER

extern float sample[Ndot];	//������
extern u8 convert_flag;	//��ͼ��־

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);

#endif
