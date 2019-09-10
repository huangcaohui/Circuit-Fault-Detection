#include "calculation.h"

unsigned long frequency = 1000;	//输出信号频率值
u16 tim3_arr = 25;	//定时器3自动重装载值
u16 tim3_psc = 105;		//定时器3分频系数
u16 Urms = 10;	//输入电压有效值
u16 dot_num;	//单位时间内采样点个数，分频系数105，所以84M/105=800Khz的计数频率，800khz/25=32000hz，计数32000次为1s

float VBrms, VArms, VDrms;	//定义AD读取通道四电压有效值
float VO1, VO2;	//定义输出无负载和有负载时AD读取电压
float VOdc;	//输出无负载时直流分量
float Ri, Ro, Av;	//定义测量值

u8 switch_flag;	//继电器打开标志
unsigned long scan_frequency;

float time_fs[4][Ndot]; //用于拷贝采样点的时域数组

void cal_param(void)
{
	frequency=1000;
	Freq_convert(frequency);	//开启频率
	delay_ms(500);	//等待稳态
	
	data_copy();	//数据拷贝
			
	calRi(time_fs[0], time_fs[1]);	//计算输入电阻
	calRo(time_fs[3]);	//计算输出电阻
	calAv();	//计算增益
}
void data_copy(void)
{
	int i, j;
	
	ad_num = 0;	//AD读取数据清0
	convert_flag = 0;	//清除标志位
	
	while(convert_flag == 1);	//等待转换完成
	
	for(i = 0; i < 4; ++i)
	{
		int *point = ad7606_data[2*i + 1];
		for(j = 0; j < Ndot; ++j)
		{
			time_fs[i][j] = *point++;	//将AD读取的信号拷贝到时域的数组中
		}
	}
}

//计算输入电阻
void calRi(float *input_dotA, float *input_dotB)
{
	float_fft(input_dotA, 6);	//快速傅里叶变化
	VArms = vk[frequency/(dot_num/Ndot)];	//读取A点一次谐波谱线的有效值
	
	float_fft(input_dotB, 6);	//快速傅里叶变化
	VBrms = vk[frequency/(dot_num/Ndot)];	//读取B点一次谐波谱线的有效值
	
	Ri = VArms*R1/(VBrms - VArms)*1.1;	//求输入电阻，有效值1/√2相消，按幅值计算
}

//计算输出电阻
void calRo(float *input_dotD)
{
	float_fft(input_dotD, 6);	//快速傅里叶变化
	VDrms = vk[frequency/(dot_num/Ndot)];	//读取D点无负载一次谐波谱线的有效值
	
	VO1 = (1+100/51)*VDrms*79.836776;
	
	RELAY = 1;	//继电器闭合后读取电压值
	delay_ms(500);
	
	data_copy();
	
	float_fft(time_fs[3], 6);	//快速傅里叶变化
	VDrms = vk[frequency/(dot_num/Ndot)];	//读取D点有负载一次谐波谱线的有效值
	
	VO2 = (1+100/51)*VDrms*79.836776;
	
	Ro = (VO1/VO2 - 1)*R2;
	Ro = 150e3*Ro/(150e3 - Ro);
	
	RELAY = 0;	//继电器打开
}	

void calRo_fault(float *input_dotD)
{
	float_fft(input_dotD, 6);	//快速傅里叶变化
	VDrms = vk[frequency/(dot_num/Ndot)];	//读取D点无负载一次谐波谱线的有效值
	
	VO1 = (1+100/51)*VDrms*79.836776;
	VOdc = (1+100/51)*vk[0]*79.836776*2;
}

//计算电路增益
void calAv(void)
{
	Av = VO1/VArms;
}

float Vrms;	//存储AD读取点的有效值
float fres[2195];	//扫频模式频率
float Avs[2195];	//扫频模式增益
float fsl, fsh;	//下限截止频率与上限截止频率
float average;	//增益最大时均值

//计算幅频值
void cal_amp_fre(void)
{
	long fre;	//扫描频率
	u16 num = 0;	//AD存储点个数计数
	int i;
	
	//低频区
	for(fre = 10; fre <= 10000; fre += 10)	//1000点
	{
		Freq_convert(fre);
		delay_us(500);
		
		ad_num = 0;	//AD读取数据清0
		calc_flag = 0;
		
		while(calc_flag == 1);
		
		Vrms = ad7606_data[5][0];
		
		Avs[num] = 2.0*Vrms/VArms;	//校准
		fres[num] = fre;
		++num;
	}
	
	//中频区
	for(fre = 10000; fre < 50000; fre += 40)	//1000点
	{
		Freq_convert(fre);
		delay_us(500);
		
		ad_num = 0;	//AD读取数据清0
		calc_flag = 0;
		
		while(calc_flag == 1);
		
		Vrms = ad7606_data[5][0];
		
		Avs[num] = 2.0*Vrms/VArms;	//校准
		fres[num] = fre;
		++num;
	}
	
	//高频区
	for(fre = 50000; fre < 2000000; fre += 10000)	//195点
	{
		Urms =11;
		Write_Amplitude(Urms);
		
		Freq_convert(fre);
		delay_us(500);
		Urms =10;
		ad_num = 0;	//AD读取数据清0
		calc_flag = 0;
		
		while(calc_flag == 1);
		
		Vrms = ad7606_data[5][0];
		
		Avs[num] = 1.85*Vrms/VArms;	//校准
		fres[num] = fre;
		++num;
	}
	
	//计算增益最大值取平均
	for(i = 1000; i < 1000 + 1000; ++i)
	{
		average += Avs[i];
	}
	average /= 1000;
	
	//计算下限截止频率
	for(i = 1000; i > 0; --i)
	{
		if(Avs[i] < 0.707*average)
		{
			break;
		}
		fsl = fres[i-2];
	}
	
	//计算上限截止频率
	for(i = 2000 ; i < 2195; ++i)
	{
		if(Avs[i] < 0.707*average)
		{
			break;
		}
		fsh = fres[i+5];
	}
}
