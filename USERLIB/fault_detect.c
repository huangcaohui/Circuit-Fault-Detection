#include "fault_detect.h"

float mould[4][8];	//ģ��
float ind_mould[8];	//����ģ��
u8 states[7][3];	//״̬

void mould_Init(void)
{
	//10mv,20hz
	frequency = 20;
	tim3_arr = 25;	//��ʱ��3�Զ���װ��ֵ
	tim3_psc = 525*5;		//��ʱ��3��Ƶϵ��,��Ƶϵ��525������84M/525=160Khz�ļ���Ƶ�ʣ�160khz/25=6400hz
	TIM3_Int_Init(tim3_arr-1, tim3_psc-1);
	dot_num = 84e6/tim3_arr/tim3_psc;
	
	Write_Amplitude(10); //������ֵ
	Freq_convert(frequency);	//����Ƶ��
	delay_ms(1000);	//�ȴ���̬
	
	data_copy();	//���ݿ���
			
	calRi(time_fs[0], time_fs[1]);	//�����������
	calRo_fault(time_fs[3]);	//�����������
	calAv();	//��������
	
	mould[0][0] = VArms;	//A����Чֵ
	mould[0][1] = VBrms;	//B����Чֵ
	mould[0][2] = VO1;	//�����Чֵ
	mould[0][3] = VOdc;	//���ֱ������
	mould[0][4] = Ri;	//�������
	mould[0][5] = Av;	//����
	
	frequency = 20;
	Write_Amplitude(10); //������ֵ
	Freq_convert(frequency);	//����Ƶ��
	delay_ms(500);	//�ȴ���̬

    ad_num = 0;	//AD��ȡ������0
	calc_flag = 0;		
	while(calc_flag == 1);		
	mould[0][6] = ad7606_data[5][0];	//�����Чֵ
	
	//10mv,1khz
	frequency = 1000;
	tim3_arr = 25;	//��ʱ��3�Զ���װ��ֵ
	tim3_psc = 105;		//��ʱ��3��Ƶϵ��,��Ƶϵ��105������84M/105=800Khz�ļ���Ƶ�ʣ�800khz/25=32000hz
	TIM3_Int_Init(tim3_arr-1, tim3_psc-1);
	dot_num = 84e6/tim3_arr/tim3_psc;
	
	Write_Amplitude(10); //������ֵ
	Freq_convert(frequency);	//����Ƶ��
	delay_ms(500);	//�ȴ���̬
	
	data_copy();	//���ݿ���
			
	calRi(time_fs[0], time_fs[1]);	//�����������
	calRo_fault(time_fs[3]);	//�����������
	calAv();	//��������
	
	mould[1][0] = VArms;	//A����Чֵ
	mould[1][1] = VBrms;	//B����Чֵ
	mould[1][2] = VO1;	//�����Чֵ
	mould[1][3] = VOdc;	//���ֱ������
	mould[1][4] = Ri;	//�������
	mould[1][5] = Av;	//����
	
	//10mv,100Khz
	frequency = 100000;
	Write_Amplitude(20); //������ֵ
	Freq_convert(frequency);	//����Ƶ��
	delay_ms(500);	//�ȴ���̬

    ad_num = 0;	//AD��ȡ������0
	calc_flag = 0;		
	while(calc_flag == 1);		
	mould[2][2] = ad7606_data[5][0];	//�����Чֵ
	
	//����������״̬
	frequency = 1000;	//����ź�Ƶ��ֵ
	tim3_arr = 25;	//��ʱ��3�Զ���װ��ֵ
	tim3_psc = 105;		//��ʱ��3��Ƶϵ��
	Urms = 10;	//�����ѹ��Чֵ	

	frequency = 5;
	Write_Amplitude(20); //������ֵ
	Freq_convert(frequency);	//����Ƶ��
	delay_ms(500);	//�ȴ���̬

    ad_num = 0;	//AD��ȡ������0
	calc_flag = 0;		
	while(calc_flag == 1);		
	mould[3][6] = ad7606_data[5][0];	//�����Чֵ
}

void fault_analyse(void)
{
	//����״̬���ԣ�10mv,1khz
	frequency = 1000;
	Write_Amplitude(10); //������ֵ
	Freq_convert(frequency);	//����Ƶ��
	delay_ms(500);	//�ȴ���̬
	
	data_copy();	//���ݿ���
			
	calRi(time_fs[0], time_fs[1]);	//�����������
	calRo_fault(time_fs[3]);	//�����������
	calAv();	//��������
	
	if(VOdc - mould[1][3] > 20000)
	{
		if(VArms < mould[1][0]*0.5)
		{
			if(VOdc - mould[1][3] < 65565)
			{
				display_fault(1);	//R1��·
				return;
			}
			else
			{
				display_fault(2);	//R2��·
				return;
			}
		}
		else if(VArms > 1.1*mould[1][0])
		{
			if(VArms > 9.25 && VArms < 9.4)
			{
				display_fault(5);	//R1��·
				return;
			}
			if(VArms > 9.0 && VArms < 9.2)
			{
				display_fault(8);	//R4��·
				return;
			}
		}
		else
		{
			display_fault(3);	//R3��·
			return;
		}
	}
	else if(mould[1][3] - VOdc> 20000)
	{
		if(VOdc > mould[1][3]*0.4)
		{
			display_fault(6);	//R2��·
			return;
		}
		else
		{
			if((VOdc > mould[1][3]*0.04) && (VOdc < mould[1][3]*0.1))
			{
				display_fault(7);	//R3��·
				return;
			}
			else
			{
				display_fault(4);	//R4��·
				return;
			}
		}
	}
	else
	{
		if(Av<1) 
		{   
			display_fault(12);	//c1��·
			return;    
		}
		else if(Av<10)  
		{
			display_fault(13); //c2��·
			return ;
	    }
		
		frequency = 100000;
		Write_Amplitude(20); //������ֵ
		Freq_convert(frequency);	//����Ƶ��
		delay_ms(500);	//�ȴ���̬

		ad_num = 0;	//AD��ȡ������0
		calc_flag = 0;		
		while(calc_flag == 1);		
		Av = ad7606_data[5][0];	//�����Чֵ
		
		if( ((Av/mould[2][2])>1.1) && (Av/mould[2][2])<1.5 )
		{
		   display_fault(14);	//c3��·
		   return;
		}
		else if( ((Av/mould[2][2])>0.6) && (Av/mould[2][2])<0.9 )
		{
		
		   display_fault(11);	//c3����	
           return;		
		}

		frequency = 20;
		tim3_arr = 25;	//��ʱ��3�Զ���װ��ֵ
		tim3_psc = 525*5;		//��ʱ��3��Ƶϵ��,��Ƶϵ��525������84M/525=160Khz�ļ���Ƶ�ʣ�160khz/25=6400hz
		TIM3_Int_Init(tim3_arr-1, tim3_psc-1);
		dot_num = 84e6/tim3_arr/tim3_psc;
		
		Write_Amplitude(10); //������ֵ
		Freq_convert(frequency);	//����Ƶ��
		delay_ms(800);	//�ȴ���̬
		
		data_copy();	//���ݿ���
			
	    calRi(time_fs[0], time_fs[1]);	//�����������
		calRo_fault(time_fs[3]);	//�����������
		calAv();	//��������
		
		tim3_arr = 25;	//��ʱ��3�Զ���װ��ֵ
		tim3_psc = 105;		//��ʱ��3��Ƶϵ��,��Ƶϵ��525������84M/525=160Khz�ļ���Ƶ�ʣ�160khz/25=6400hz
		TIM3_Int_Init(tim3_arr-1, tim3_psc-1);
		dot_num = 84e6/tim3_arr/tim3_psc;
	
		if((Av - mould[0][5] > 5) && (Av-mould[0][5] < 15))
		{
		   display_fault(10); //c2����	
           return;		
		}
		if(Av > mould[0][5] + 0.5)
		{
			display_fault(9); //c1����	
			return;
		}	
	}
	
	//��ʾ����
	display_fault(0);
}
