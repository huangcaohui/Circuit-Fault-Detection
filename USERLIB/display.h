#ifndef __DISPLAY_H
#define __DISPLAY_H	 

#include "sys.h"
#include "lcd.h"
#include "timer.h"
#include "math.h"
#include "calculation.h"

#define LEFT_EDGE	20	//��߼��
#define	RIGHT_EDGE	20	//�ұ߼��
#define TOP_EDGE	40	//�ϱ߼��
#define	BOTTOM_EDGE	40	//�±߼��

#define POINT_NUM	240-RIGHT_EDGE-LEFT_EDGE

#define POINT_X(x)	LEFT_EDGE+(x)	//��߾࣬xС��POINT_NUM

#define POINT_Y(y)	320-BOTTOM_EDGE-(y)/32767*(320-TOP_EDGE-BOTTOM_EDGE)

void Display_Init(void);
void time_display(float *input_dot);	//��ʾʱ��ԭ����
void fir_display(float *input_dot);	//��ʾʱ���˲�����
void freq_display(float *input_dot);	//��ʾƵ����

#define LEFT_EDGE1	  20	//Excel1����ϵ��߼��
#define	RIGHT_EDGE1  	20	//Excel1����ϵ�ұ߼��
#define TOP_EDGE1	    140	//Excel1����ϵ�ϱ߼��
#define	BOTTOM_EDGE1	20	//Excel1����ϵ�±߼��

#define POINT_NUM1	240-RIGHT_EDGE1-LEFT_EDGE1

#define POINT1_X(x)	LEFT_EDGE1+(x)	//��߾࣬xС��POINT_NUM

#define POINT1_Y(y)	320-BOTTOM_EDGE1-(y)/32767*(320-TOP_EDGE1-BOTTOM_EDGE1)

void display_param_form(void);
void display_fault_form(void);
void display_param(void);
void display_amp_fre(void);
void display_fault(int y);

#endif
