/*-----------------------------------------------
  名称：AD9854并口驱动
  编写：Liu
  日期：2014.3
  修改：无
  内容：
------------------------------------------------*/
#ifndef __AD9910_H__
#define __AD9910_H__

#include "stm32f4xx.h"

#define uchar unsigned char
#define uint  unsigned int	
#define ulong  unsigned long int

//新增IO
//#define SDO PAout(4)
//#define PDCLK PCout(6)//PDCLK
//#define PLL_LOCK PCout(7)//悬空
//#define SYNC_CLK PCout(9)//悬空
//#define TXENABLE PCout(11)//TXEN
//#define RSO PCout(12)//RAM-SWP-OVR
#define AD9910_PWR PCout(13)//0
//原始IO

//#define AD9910_SDIO PAout(2)
//#define SCLK PCout(2)

//#define UP_DAT PAout(3)
//#define CS  PAout(7)
//#define MAS_REST PCout(3)

//#define DRCTL  PAout(4)
//#define DRHOLD PAout(5)

//#define PROFILE0 PAout(6)
//#define PROFILE1 PAout(1)
//#define PROFILE2 PCout(4)



#define AD9910_SDIO PBout(6)//#define AD9910_SDIO PAout(2)
#define SCLK PBout(8)       //#define SCLK PCout(2)

#define UP_DAT PBout(5)     //#define UP_DAT PAout(3)
#define CSB  PBout(9)        //#define CSB  PAout(7)
#define MAS_REST PGout(14)   //#define MAS_REST PCout(3)

#define DRCTL  PBout(4)     //#define DRCTL  PAout(4)
#define DRHOLD PBout(7)     //#define DRHOLD PAout(5)

#define PROFILE0 PGout(15)   //#define PROFILE0 PAout(6)
#define PROFILE1 PBout(3)   //#define PROFILE1 PAout(1)
#define PROFILE2 PGout(13)   //#define PROFILE2 PCout(4)

//#define DROVER PCout(2)     //#define DROVER PCout(2)

//#define OSK PCout(8)
void AD9110_IOInit(void);
void Init_ad9910(void);
void txd_8bit(uchar txdat);
void Txcfr(void);
void Freq_convert(ulong Freq); //写频率，输入范围：1-400 000 000Hz
void Write_Amplitude(uint Amp); //写幅度，输入范围：1-650 mV
//扫频波下限频率，上限频率，频率步进（单位：Hz），步进时间间隔（单位：ns，范围：4*(1~65536)ns）
void SweepFre(ulong SweepMinFre, ulong SweepMaxFre, ulong SweepStepFre, ulong SweepTime);
void Square_wave(uint Sample_interval);//方波，采样时间间隔输入范围：4*(1~65536)ns


#endif


