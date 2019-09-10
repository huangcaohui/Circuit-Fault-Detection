#include "fault_detect.h"

float mould[4][8];	//模板
float ind_mould[8];	//单个模板
u8 states[7][3];	//状态

void mould_Init(void)
{
	//10mv,20hz
	frequency = 20;
	tim3_arr = 25;	//定时器3自动重装载值
	tim3_psc = 525*5;		//定时器3分频系数,分频系数525，所以84M/525=160Khz的计数频率，160khz/25=6400hz
	TIM3_Int_Init(tim3_arr-1, tim3_psc-1);
	dot_num = 84e6/tim3_arr/tim3_psc;
	
	Write_Amplitude(10); //均方根值
	Freq_convert(frequency);	//开启频率
	delay_ms(1000);	//等待稳态
	
	data_copy();	//数据拷贝
			
	calRi(time_fs[0], time_fs[1]);	//计算输入电阻
	calRo_fault(time_fs[3]);	//计算输出电阻
	calAv();	//计算增益
	
	mould[0][0] = VArms;	//A点有效值
	mould[0][1] = VBrms;	//B点有效值
	mould[0][2] = VO1;	//输出有效值
	mould[0][3] = VOdc;	//输出直流分量
	mould[0][4] = Ri;	//输入电阻
	mould[0][5] = Av;	//增益
	
	frequency = 20;
	Write_Amplitude(10); //均方根值
	Freq_convert(frequency);	//开启频率
	delay_ms(500);	//等待稳态

    ad_num = 0;	//AD读取数据清0
	calc_flag = 0;		
	while(calc_flag == 1);		
	mould[0][6] = ad7606_data[5][0];	//输出有效值
	
	//10mv,1khz
	frequency = 1000;
	tim3_arr = 25;	//定时器3自动重装载值
	tim3_psc = 105;		//定时器3分频系数,分频系数105，所以84M/105=800Khz的计数频率，800khz/25=32000hz
	TIM3_Int_Init(tim3_arr-1, tim3_psc-1);
	dot_num = 84e6/tim3_arr/tim3_psc;
	
	Write_Amplitude(10); //均方根值
	Freq_convert(frequency);	//开启频率
	delay_ms(500);	//等待稳态
	
	data_copy();	//数据拷贝
			
	calRi(time_fs[0], time_fs[1]);	//计算输入电阻
	calRo_fault(time_fs[3]);	//计算输出电阻
	calAv();	//计算增益
	
	mould[1][0] = VArms;	//A点有效值
	mould[1][1] = VBrms;	//B点有效值
	mould[1][2] = VO1;	//输出有效值
	mould[1][3] = VOdc;	//输出直流分量
	mould[1][4] = Ri;	//输入电阻
	mould[1][5] = Av;	//增益
	
	//10mv,100Khz
	frequency = 100000;
	Write_Amplitude(20); //均方根值
	Freq_convert(frequency);	//开启频率
	delay_ms(500);	//等待稳态

    ad_num = 0;	//AD读取数据清0
	calc_flag = 0;		
	while(calc_flag == 1);		
	mould[2][2] = ad7606_data[5][0];	//输出有效值
	
	//回正常工作状态
	frequency = 1000;	//输出信号频率值
	tim3_arr = 25;	//定时器3自动重装载值
	tim3_psc = 105;		//定时器3分频系数
	Urms = 10;	//输入电压有效值	

	frequency = 5;
	Write_Amplitude(20); //均方根值
	Freq_convert(frequency);	//开启频率
	delay_ms(500);	//等待稳态

    ad_num = 0;	//AD读取数据清0
	calc_flag = 0;		
	while(calc_flag == 1);		
	mould[3][6] = ad7606_data[5][0];	//输出有效值
}

void fault_analyse(void)
{
	//正常状态测试，10mv,1khz
	frequency = 1000;
	Write_Amplitude(10); //均方根值
	Freq_convert(frequency);	//开启频率
	delay_ms(500);	//等待稳态
	
	data_copy();	//数据拷贝
			
	calRi(time_fs[0], time_fs[1]);	//计算输入电阻
	calRo_fault(time_fs[3]);	//计算输出电阻
	calAv();	//计算增益
	
	if(VOdc - mould[1][3] > 20000)
	{
		if(VArms < mould[1][0]*0.5)
		{
			if(VOdc - mould[1][3] < 65565)
			{
				display_fault(1);	//R1短路
				return;
			}
			else
			{
				display_fault(2);	//R2短路
				return;
			}
		}
		else if(VArms > 1.1*mould[1][0])
		{
			if(VArms > 9.25 && VArms < 9.4)
			{
				display_fault(5);	//R1开路
				return;
			}
			if(VArms > 9.0 && VArms < 9.2)
			{
				display_fault(8);	//R4开路
				return;
			}
		}
		else
		{
			display_fault(3);	//R3短路
			return;
		}
	}
	else if(mould[1][3] - VOdc> 20000)
	{
		if(VOdc > mould[1][3]*0.4)
		{
			display_fault(6);	//R2开路
			return;
		}
		else
		{
			if((VOdc > mould[1][3]*0.04) && (VOdc < mould[1][3]*0.1))
			{
				display_fault(7);	//R3开路
				return;
			}
			else
			{
				display_fault(4);	//R4短路
				return;
			}
		}
	}
	else
	{
		if(Av<1) 
		{   
			display_fault(12);	//c1断路
			return;    
		}
		else if(Av<10)  
		{
			display_fault(13); //c2断路
			return ;
	    }
		
		frequency = 100000;
		Write_Amplitude(20); //均方根值
		Freq_convert(frequency);	//开启频率
		delay_ms(500);	//等待稳态

		ad_num = 0;	//AD读取数据清0
		calc_flag = 0;		
		while(calc_flag == 1);		
		Av = ad7606_data[5][0];	//输出有效值
		
		if( ((Av/mould[2][2])>1.1) && (Av/mould[2][2])<1.5 )
		{
		   display_fault(14);	//c3断路
		   return;
		}
		else if( ((Av/mould[2][2])>0.6) && (Av/mould[2][2])<0.9 )
		{
		
		   display_fault(11);	//c3倍增	
           return;		
		}

		frequency = 20;
		tim3_arr = 25;	//定时器3自动重装载值
		tim3_psc = 525*5;		//定时器3分频系数,分频系数525，所以84M/525=160Khz的计数频率，160khz/25=6400hz
		TIM3_Int_Init(tim3_arr-1, tim3_psc-1);
		dot_num = 84e6/tim3_arr/tim3_psc;
		
		Write_Amplitude(10); //均方根值
		Freq_convert(frequency);	//开启频率
		delay_ms(800);	//等待稳态
		
		data_copy();	//数据拷贝
			
	    calRi(time_fs[0], time_fs[1]);	//计算输入电阻
		calRo_fault(time_fs[3]);	//计算输出电阻
		calAv();	//计算增益
		
		tim3_arr = 25;	//定时器3自动重装载值
		tim3_psc = 105;		//定时器3分频系数,分频系数525，所以84M/525=160Khz的计数频率，160khz/25=6400hz
		TIM3_Int_Init(tim3_arr-1, tim3_psc-1);
		dot_num = 84e6/tim3_arr/tim3_psc;
	
		if((Av - mould[0][5] > 5) && (Av-mould[0][5] < 15))
		{
		   display_fault(10); //c2倍增	
           return;		
		}
		if(Av > mould[0][5] + 0.5)
		{
			display_fault(9); //c1倍增	
			return;
		}	
	}
	
	//显示归零
	display_fault(0);
}
