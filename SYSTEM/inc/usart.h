#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200

//����봮���жϽ��գ��벻Ҫע�����º궨��
void Usart_Init(void);
void Usart1_Init(u32 bound);

#endif


