#ifndef __IWDG_H
#define __IWDG_H	 

#include "sys.h"

void IWDG_Init(u8 prer,u16 rlr);//IWDG��ʼ��
void IWDG_Feed(void);  //ι������

#endif
