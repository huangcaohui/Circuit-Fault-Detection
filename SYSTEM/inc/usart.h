#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200

//如果想串口中断接收，请不要注释以下宏定义
void Usart_Init(void);
void Usart1_Init(u32 bound);

#endif


