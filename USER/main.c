#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "timer.h"
#include "pwm.h"
#include "key.h"
#include "display.h"
#include "iwdg.h"
#include "ad9910.h"
#include "ad7606.h"
#include "exti.h"
#include "calculation.h"
#include "fault_detect.h"

extern float fir_fs[Ndot];	//�������˲������

u8 key_value = 0;	//��ǰ����ģʽ
u8 key = 0;	//�������·���ֵ
int fault_kind=0;    //��������
 
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
	
	delay_init(168);	//��ʼ����ʱ����
	delay_ms(1000);
	Usart_Init();	//��ʼ������
	EXTIX_Init();	//�ⲿ�жϳ�ʼ��
	
	LED_Init();	//��ʼ��LED 
	KEY_Init();	//������ʼ��
 	LCD_Init();	//��ʼ��LCD�ӿ�

	AD7606_Init();	//AD7606��ʼ��
	delay_ms(1000);
	Init_ad9910();
	Display_Init();	//������ʼ��LCD��ʾ
	
	TIM3_Int_Init(tim3_arr-1, tim3_psc-1);	//��ʱ��ʱ��84M����Ƶϵ��525������84M/525=160Khz�ļ���Ƶ�ʣ�160khz/25=6400hz������6400��Ϊ1s 	
	dot_num = 84e6/tim3_arr/tim3_psc;
	
	Freq_convert(frequency); //дƵ�ʣ����뷶Χ��1-400 000 000Hz
	Write_Amplitude(Urms); //д������ֵ�����뷶Χ��1-650 mV
	
	while(1)
	{  		
		//IWDG_Feed();	//ι��
		key = KEY_Scan();	//����ɨ��
		
		if(key == 1)
		{
			key_value = 1;
			display_param_form(); 	//��������
			cal_param();
			display_param();	//�����������ʾ
			cal_amp_fre();	//�����ֹƵ��
			display_amp_fre();	//��ʾƵ����������
		}
		
		if(key == 2)
		{
			key_value = 2; 
			display_fault_form(); 
			mould_Init();
			display_fault(0);	//���ϼ��ģ���ʼ��
		}
		
		if(key_value == 2)
		{
			fault_analyse();
		}
	}
}
