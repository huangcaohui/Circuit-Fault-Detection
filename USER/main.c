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

extern float fir_fs[Ndot];	//采样点滤波后输出

u8 key_value = 0;	//当前键盘模式
u8 key = 0;	//按键摁下返回值
int fault_kind=0;    //错误类型
 
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	
	delay_init(168);	//初始化延时函数
	delay_ms(1000);
	Usart_Init();	//初始化串口
	EXTIX_Init();	//外部中断初始化
	
	LED_Init();	//初始化LED 
	KEY_Init();	//按键初始化
 	LCD_Init();	//初始化LCD接口

	AD7606_Init();	//AD7606初始化
	delay_ms(1000);
	Init_ad9910();
	Display_Init();	//开机初始化LCD显示
	
	TIM3_Int_Init(tim3_arr-1, tim3_psc-1);	//定时器时钟84M，分频系数525，所以84M/525=160Khz的计数频率，160khz/25=6400hz，计数6400次为1s 	
	dot_num = 84e6/tim3_arr/tim3_psc;
	
	Freq_convert(frequency); //写频率，输入范围：1-400 000 000Hz
	Write_Amplitude(Urms); //写均方根值，输入范围：1-650 mV
	
	while(1)
	{  		
		//IWDG_Feed();	//喂狗
		key = KEY_Scan();	//键盘扫描
		
		if(key == 1)
		{
			key_value = 1;
			display_param_form(); 	//画参数表
			cal_param();
			display_param();	//计算参数，显示
			cal_amp_fre();	//计算截止频率
			display_amp_fre();	//显示频幅特性曲线
		}
		
		if(key == 2)
		{
			key_value = 2; 
			display_fault_form(); 
			mould_Init();
			display_fault(0);	//故障检测模板初始化
		}
		
		if(key_value == 2)
		{
			fault_analyse();
		}
	}
}
