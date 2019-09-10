#include "calculation.h"

unsigned long frequency = 1000;	//����ź�Ƶ��ֵ
u16 tim3_arr = 25;	//��ʱ��3�Զ���װ��ֵ
u16 tim3_psc = 105;		//��ʱ��3��Ƶϵ��
u16 Urms = 10;	//�����ѹ��Чֵ
u16 dot_num;	//��λʱ���ڲ������������Ƶϵ��105������84M/105=800Khz�ļ���Ƶ�ʣ�800khz/25=32000hz������32000��Ϊ1s

float VBrms, VArms, VDrms;	//����AD��ȡͨ���ĵ�ѹ��Чֵ
float VO1, VO2;	//��������޸��غ��и���ʱAD��ȡ��ѹ
float VOdc;	//����޸���ʱֱ������
float Ri, Ro, Av;	//�������ֵ

u8 switch_flag;	//�̵����򿪱�־
unsigned long scan_frequency;

float time_fs[4][Ndot]; //���ڿ����������ʱ������

void cal_param(void)
{
	frequency=1000;
	Freq_convert(frequency);	//����Ƶ��
	delay_ms(500);	//�ȴ���̬
	
	data_copy();	//���ݿ���
			
	calRi(time_fs[0], time_fs[1]);	//�����������
	calRo(time_fs[3]);	//�����������
	calAv();	//��������
}
void data_copy(void)
{
	int i, j;
	
	ad_num = 0;	//AD��ȡ������0
	convert_flag = 0;	//�����־λ
	
	while(convert_flag == 1);	//�ȴ�ת�����
	
	for(i = 0; i < 4; ++i)
	{
		int *point = ad7606_data[2*i + 1];
		for(j = 0; j < Ndot; ++j)
		{
			time_fs[i][j] = *point++;	//��AD��ȡ���źſ�����ʱ���������
		}
	}
}

//�����������
void calRi(float *input_dotA, float *input_dotB)
{
	float_fft(input_dotA, 6);	//���ٸ���Ҷ�仯
	VArms = vk[frequency/(dot_num/Ndot)];	//��ȡA��һ��г�����ߵ���Чֵ
	
	float_fft(input_dotB, 6);	//���ٸ���Ҷ�仯
	VBrms = vk[frequency/(dot_num/Ndot)];	//��ȡB��һ��г�����ߵ���Чֵ
	
	Ri = VArms*R1/(VBrms - VArms)*1.1;	//��������裬��Чֵ1/��2����������ֵ����
}

//�����������
void calRo(float *input_dotD)
{
	float_fft(input_dotD, 6);	//���ٸ���Ҷ�仯
	VDrms = vk[frequency/(dot_num/Ndot)];	//��ȡD���޸���һ��г�����ߵ���Чֵ
	
	VO1 = (1+100/51)*VDrms*79.836776;
	
	RELAY = 1;	//�̵����պϺ��ȡ��ѹֵ
	delay_ms(500);
	
	data_copy();
	
	float_fft(time_fs[3], 6);	//���ٸ���Ҷ�仯
	VDrms = vk[frequency/(dot_num/Ndot)];	//��ȡD���и���һ��г�����ߵ���Чֵ
	
	VO2 = (1+100/51)*VDrms*79.836776;
	
	Ro = (VO1/VO2 - 1)*R2;
	Ro = 150e3*Ro/(150e3 - Ro);
	
	RELAY = 0;	//�̵�����
}	

void calRo_fault(float *input_dotD)
{
	float_fft(input_dotD, 6);	//���ٸ���Ҷ�仯
	VDrms = vk[frequency/(dot_num/Ndot)];	//��ȡD���޸���һ��г�����ߵ���Чֵ
	
	VO1 = (1+100/51)*VDrms*79.836776;
	VOdc = (1+100/51)*vk[0]*79.836776*2;
}

//�����·����
void calAv(void)
{
	Av = VO1/VArms;
}

float Vrms;	//�洢AD��ȡ�����Чֵ
float fres[2195];	//ɨƵģʽƵ��
float Avs[2195];	//ɨƵģʽ����
float fsl, fsh;	//���޽�ֹƵ�������޽�ֹƵ��
float average;	//�������ʱ��ֵ

//�����Ƶֵ
void cal_amp_fre(void)
{
	long fre;	//ɨ��Ƶ��
	u16 num = 0;	//AD�洢���������
	int i;
	
	//��Ƶ��
	for(fre = 10; fre <= 10000; fre += 10)	//1000��
	{
		Freq_convert(fre);
		delay_us(500);
		
		ad_num = 0;	//AD��ȡ������0
		calc_flag = 0;
		
		while(calc_flag == 1);
		
		Vrms = ad7606_data[5][0];
		
		Avs[num] = 2.0*Vrms/VArms;	//У׼
		fres[num] = fre;
		++num;
	}
	
	//��Ƶ��
	for(fre = 10000; fre < 50000; fre += 40)	//1000��
	{
		Freq_convert(fre);
		delay_us(500);
		
		ad_num = 0;	//AD��ȡ������0
		calc_flag = 0;
		
		while(calc_flag == 1);
		
		Vrms = ad7606_data[5][0];
		
		Avs[num] = 2.0*Vrms/VArms;	//У׼
		fres[num] = fre;
		++num;
	}
	
	//��Ƶ��
	for(fre = 50000; fre < 2000000; fre += 10000)	//195��
	{
		Urms =11;
		Write_Amplitude(Urms);
		
		Freq_convert(fre);
		delay_us(500);
		Urms =10;
		ad_num = 0;	//AD��ȡ������0
		calc_flag = 0;
		
		while(calc_flag == 1);
		
		Vrms = ad7606_data[5][0];
		
		Avs[num] = 1.85*Vrms/VArms;	//У׼
		fres[num] = fre;
		++num;
	}
	
	//�����������ֵȡƽ��
	for(i = 1000; i < 1000 + 1000; ++i)
	{
		average += Avs[i];
	}
	average /= 1000;
	
	//�������޽�ֹƵ��
	for(i = 1000; i > 0; --i)
	{
		if(Avs[i] < 0.707*average)
		{
			break;
		}
		fsl = fres[i-2];
	}
	
	//�������޽�ֹƵ��
	for(i = 2000 ; i < 2195; ++i)
	{
		if(Avs[i] < 0.707*average)
		{
			break;
		}
		fsh = fres[i+5];
	}
}
