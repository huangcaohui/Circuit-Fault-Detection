#include "exti.h"

u8 calc_flag = 0;

void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource15);//PF15 ���ӵ��ж���15
	
	/* ����EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;//LINE15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE15
	EXTI_Init(&EXTI_InitStructure);//����
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�15
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
}

void EXTI15_10_IRQHandler(void)
{
	 if(EXTI_GetFlagStatus(EXTI_Line15) == SET)
	 {
		 AD7606_read();	//��ȡ����
		 ++ad_num;
		 calc_flag = 1;
		 if(ad_num == Ndot)
		 {
		 	 convert_flag = 1;
			 ad_num = 0;
		 }
	 }
	 		 
	EXTI_ClearITPendingBit(EXTI_Line15);  //���LINE15�ϵ��жϱ�־λ 
}
