#ifndef __DISPLAY_H
#define __DISPLAY_H	 

#include "sys.h"
#include "lcd.h"
#include "timer.h"
#include "math.h"
#include "calculation.h"

#define LEFT_EDGE	20	//左边间距
#define	RIGHT_EDGE	20	//右边间距
#define TOP_EDGE	40	//上边间距
#define	BOTTOM_EDGE	40	//下边间距

#define POINT_NUM	240-RIGHT_EDGE-LEFT_EDGE

#define POINT_X(x)	LEFT_EDGE+(x)	//左边距，x小于POINT_NUM

#define POINT_Y(y)	320-BOTTOM_EDGE-(y)/32767*(320-TOP_EDGE-BOTTOM_EDGE)

void Display_Init(void);
void time_display(float *input_dot);	//显示时域原波形
void fir_display(float *input_dot);	//显示时域滤波波形
void freq_display(float *input_dot);	//显示频域波形

#define LEFT_EDGE1	  20	//Excel1坐标系左边间距
#define	RIGHT_EDGE1  	20	//Excel1坐标系右边间距
#define TOP_EDGE1	    140	//Excel1坐标系上边间距
#define	BOTTOM_EDGE1	20	//Excel1坐标系下边间距

#define POINT_NUM1	240-RIGHT_EDGE1-LEFT_EDGE1

#define POINT1_X(x)	LEFT_EDGE1+(x)	//左边距，x小于POINT_NUM

#define POINT1_Y(y)	320-BOTTOM_EDGE1-(y)/32767*(320-TOP_EDGE1-BOTTOM_EDGE1)

void display_param_form(void);
void display_fault_form(void);
void display_param(void);
void display_amp_fre(void);
void display_fault(int y);

#endif
