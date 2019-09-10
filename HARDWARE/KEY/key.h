#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
#include "delay.h" 
#include "led.h"

/*下面的方式是通过直接操作库函数方式读取IO*/
//#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
//#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
//#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
//#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0


/*下面方式是通过位带操作方式读取IO*/

#define WK_UP PAin(0)		//PA0
#define KEY0 PEin(4)		//PE4
#define KEY1 PEin(3)   	//PE3

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   4

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(void);  		//按键扫描函数
u8 KEY0_Scan(void);

#endif
