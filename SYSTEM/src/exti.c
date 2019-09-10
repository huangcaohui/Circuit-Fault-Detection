#include "exti.h"

u8 calc_flag = 0;

void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource15);//PF15 连接到中断线15
	
	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;//LINE15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE15
	EXTI_Init(&EXTI_InitStructure);//配置
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断15
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

void EXTI15_10_IRQHandler(void)
{
	 if(EXTI_GetFlagStatus(EXTI_Line15) == SET)
	 {
		 AD7606_read();	//读取数据
		 ++ad_num;
		 calc_flag = 1;
		 if(ad_num == Ndot)
		 {
		 	 convert_flag = 1;
			 ad_num = 0;
		 }
	 }
	 		 
	EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15上的中断标志位 
}
