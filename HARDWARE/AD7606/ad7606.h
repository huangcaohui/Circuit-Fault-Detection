#ifndef __AD7606_H
#define __AD7606_H

#include "sys.h"
#include "floatfft.h"

#define	BUSY	PFin(15)
#define	CS	PAout(1)
#define RD	PAout(2)
#define	COV	PAout(3)
#define	OS2	PAout(4)
#define	OS1	PAout(5)
#define	RST	PAout(6)
#define	OS0	PAout(7)

extern u16	ad_num;	//AD读取个数计数
extern int ad7606_data[8][Ndot];	//存储AD读取数据

void AD7606_GPIO_Init(void);
void AD7606_Init(void);
void AD7606_read(void);

#endif
