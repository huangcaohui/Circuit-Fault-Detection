#ifndef __CALCULATION_H__
#define __CALCULATION_H__

#include <math.h>
#include "sys.h"
#include "floatfft.h"
#include "ad9910.h"
#include "timer.h"
#include "exti.h"

#define R1	1000
#define R2	2000

#define	RELAY	PBout(10)

extern u16 dot_num;
extern unsigned long frequency;
extern u16 tim3_arr;
extern u16 tim3_psc;
extern u16 Urms;

extern float fsl, fsh;	//���޽�ֹƵ�������޽�ֹƵ��
extern float fres[2195];	//ɨƵģʽƵ��
extern float Avs[2195];	//ɨƵģʽ����
extern float Ri, Ro, Av;	//������裬������裬����
extern float VBrms, VArms, VDrms;	//AD��ȡͨ���ĵ�ѹ��Чֵ
extern float VO1, VO2;	//����޸��غ��и���ʱAD��ȡ��ѹ
extern float VOdc;	//����޸���ʱֱ������
extern float time_fs[4][Ndot]; //���ڿ����������ʱ������

void cal_param(void);	//�������
void data_copy(void);	//���ݿ���
void calRi(float *input_dotA, float *input_dotB);	//�����������
void calRo(float *input_dotD);	//�����������
void calRo_fault(float *input_dotD);	//��������������
void calAv(void);	//�����·����
void cal_amp_fre(void);	//�����Ƶֵ

#endif
