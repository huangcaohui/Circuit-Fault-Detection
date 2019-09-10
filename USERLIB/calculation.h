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

extern float fsl, fsh;	//下限截止频率与上限截止频率
extern float fres[2195];	//扫频模式频率
extern float Avs[2195];	//扫频模式增益
extern float Ri, Ro, Av;	//输入电阻，输出电阻，增益
extern float VBrms, VArms, VDrms;	//AD读取通道四电压有效值
extern float VO1, VO2;	//输出无负载和有负载时AD读取电压
extern float VOdc;	//输出无负载时直流分量
extern float time_fs[4][Ndot]; //用于拷贝采样点的时域数组

void cal_param(void);	//计算参数
void data_copy(void);	//数据拷贝
void calRi(float *input_dotA, float *input_dotB);	//计算输入电阻
void calRo(float *input_dotD);	//计算输出电阻
void calRo_fault(float *input_dotD);	//计算故障输出电阻
void calAv(void);	//计算电路增益
void cal_amp_fre(void);	//计算幅频值

#endif
