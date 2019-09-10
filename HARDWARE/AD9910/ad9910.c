#include "delay.h"
#include "ad9910.h"
 /*-----------------------------------------------
  名称：AD9910串行驱动
  编写：Liu
  日期：2014.6
  修改：无
  内容：
------------------------------------------------*/
uchar cfr1[4]={0x00,0x40,0x00,0x00};       //cfr1控制字
uchar cfr2[4]={0x01,0x00,0x00,0x00};       //cfr2控制字
const uchar cfr3[4]={0x05,0x0f,0x41,0x32}; //cfr3控制字  40M输入  25倍频  VC0=101   ICP=001;
uchar profile0[8]={0x3f,0xff, 0x00,0x00, 0x25,0x09,0x7b,0x42}; //profile0控制字 0x25,0x09,0x7b,0x42
                  //01振幅控制  23相位控制  4567频率调谐字
				  //高字节							低字节

uchar drgparameter[20]={0x00}; //DRG参数
uchar ramprofile0[8] = {0x00}; //ramprofile0控制字
extern const unsigned char ramdata_Square[4096];


//=====================================================================
void AD9110_IOInit()
{
	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOG, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//??????
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
 // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8| GPIO_Pin_9|GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//???	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);			
}

//void AD9110_IOInit(){
//	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
//	
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);
//	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//??????
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
// // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
//  
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1| GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//???	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_4;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);	

//		
//}
//======================ad9910初始化程序===============================
void Init_ad9910(void)
{	
	AD9110_IOInit();//IO初始化
	AD9910_PWR = 0;//软件拉低
	
	PROFILE2=PROFILE1=PROFILE0=0;
	DRCTL=0;DRHOLD=0;
	MAS_REST=1; 
	delay_ms(5);  
	MAS_REST=0; 

	Txcfr();
}    


//=====================================================================

//======================发送8位数据程序================================
void txd_8bit(uchar txdat)
{
	uchar i,sbt;
	sbt=0x80;
	SCLK=0;
	for (i=0;i<8;i++)
	{
		if ((txdat & sbt)==0) 
			AD9910_SDIO=0; 
		else 
			AD9910_SDIO=1;
		SCLK=1;
		sbt=sbt>>1;
		SCLK=0;
	}
}  
//=====================================================================

//======================ad9910发送cfrx控制字程序=======================
void Txcfr(void)
{
	uchar m,k;

	CSB=0;
	txd_8bit(0x00);    //发送CFR1控制字地址
	for (m=0;m<4;m++)
		txd_8bit(cfr1[m]); 
	CSB=1;  
	for (k=0;k<10;k++);
	
	CSB=0;
	txd_8bit(0x01);    //发送CFR2控制字地址
	for (m=0;m<4;m++)
		txd_8bit(cfr2[m]); 
	CSB=1;  
	for (k=0;k<10;k++);

	CSB=0;
	txd_8bit(0x02);    //发送CFR3控制字地址
	for (m=0;m<4;m++)
		txd_8bit(cfr3[m]); 
	CSB=1;
	for (k=0;k<10;k++);

	UP_DAT=1;
	for(k=0;k<10;k++);
	UP_DAT=0;
	delay_ms(1);
}         
//=====================================================================

//===================ad9910发送profile0控制字程序======================
void Txprofile(void)
{
	uchar m,k;

	CSB=0;
	txd_8bit(0x0e);    //发送profile0控制字地址
	for (m=0;m<8;m++)
		txd_8bit(profile0[m]); 
	CSB=1;
	for(k=0;k<10;k++);

	UP_DAT=1;
	for(k=0;k<10;k++);
	UP_DAT=0;
	delay_ms(1);
}         
//=====================================================================

//===================计算频偏字、频率字和发送程序======================
void Freq_convert(ulong Freq)
{   
	ulong Temp;

	
	if(Freq > 400000000)
		Freq = 400000000;
	Temp=(ulong)Freq*4.294967296; //将输入频率因子分为四个字节  4.294967296=(2^32)/1000000000 （1G 是内部时钟速度）
	profile0[7]=(uchar)Temp;
	profile0[6]=(uchar)(Temp>>8);
	profile0[5]=(uchar)(Temp>>16);
	profile0[4]=(uchar)(Temp>>24);
	Txprofile();
}
//=====================================================================

//===================计算幅度字和发送程序==============================
void Write_Amplitude(uint Amp)
{
	ulong Temp;

	Temp = (ulong)Amp*25.20615385*0.629722922*2.82842712;	   //将输入幅度因子分为两个字节  25.20615385=(2^14)/650
	if(Temp > 0x3fff)
		Temp = 0x3fff;
	Temp &= 0x3fff;
	profile0[1]=(uchar)Temp;
	profile0[0]=(uchar)(Temp>>8);
	Txprofile();
}
//=====================================================================

//======================ad9910发送DRG参数程序==========================
void Txdrg(void)
{
	uchar m,k;

	CSB=0;
	txd_8bit(0x0b);    //发送数字斜坡限制地址0x0b
	for (m=0;m<8;m++)
		txd_8bit(drgparameter[m]); 
	CSB=1;
	for(k=0;k<10;k++);
	
	CSB=0;
	txd_8bit(0x0c);    //发送数字斜坡步长地址0x0c
	for (m=8;m<16;m++)
		txd_8bit(drgparameter[m]); 
	CSB=1;
	for(k=0;k<10;k++);
	
	CSB=0;
	txd_8bit(0x0d);    //发送数字斜坡速率地址0x0d
	for (m=16;m<20;m++)
		txd_8bit(drgparameter[m]); 
	CSB=1;
	for(k=0;k<10;k++);
	
	UP_DAT=1;
	for(k=0;k<10;k++);
	UP_DAT=0;
	delay_ms(1);
}         
//=====================================================================

//=====================扫频波参数设置和发送程序========================
void SweepFre(ulong SweepMinFre, ulong SweepMaxFre, ulong SweepStepFre, ulong SweepTime)
{
	ulong Temp1, Temp2, ITemp3, DTemp3, ITemp4, DTemp4;
	Temp1 = (ulong)SweepMinFre*4.294967296;
	if(SweepMaxFre > 400000000)
		SweepMaxFre = 400000000;
	Temp2 = (ulong)SweepMaxFre*4.294967296;
	if(SweepStepFre > 400000000)
		SweepStepFre = 400000000;
	
	ITemp3 = (ulong)SweepStepFre*4.294967296;
	DTemp3 = ITemp3;
	
	ITemp4 = (ulong)SweepTime/4; //1GHz/4, 单位：ns
	if(ITemp4 > 0xffff)
		ITemp4 = 0xffff;
	DTemp4 = ITemp4;
	
	//扫频上下限
	drgparameter[7]=(uchar)Temp1;
	drgparameter[6]=(uchar)(Temp1>>8);
	drgparameter[5]=(uchar)(Temp1>>16);
	drgparameter[4]=(uchar)(Temp1>>24);
	drgparameter[3]=(uchar)Temp2;
	drgparameter[2]=(uchar)(Temp2>>8);
	drgparameter[1]=(uchar)(Temp2>>16);
	drgparameter[0]=(uchar)(Temp2>>24);
	//频率步进（单位：Hz）
	drgparameter[15]=(uchar)ITemp3;
	drgparameter[14]=(uchar)(ITemp3>>8);
	drgparameter[13]=(uchar)(ITemp3>>16);
	drgparameter[12]=(uchar)(ITemp3>>24);
	drgparameter[11]=(uchar)DTemp3;
	drgparameter[10]=(uchar)(DTemp3>>8);
	drgparameter[9]=(uchar)(DTemp3>>16);
	drgparameter[8]=(uchar)(DTemp3>>24);
	//步进时间间隔（单位：us）
	drgparameter[19]=(uchar)ITemp4;
	drgparameter[18]=(uchar)(ITemp4>>8);
	drgparameter[17]=(uchar)DTemp4;
	drgparameter[16]=(uchar)(DTemp4>>8);
	//发送DRG参数
	Txdrg();
}
//=====================================================================

//=================ad9910发送ramprofile0控制字程序=====================
void Txramprofile(void)
{
	uchar m,k;

	CSB=0;
	txd_8bit(0x0e);    //发送ramprofile0控制字地址
	for (m=0;m<8;m++)
		txd_8bit(ramprofile0[m]); 
	CSB=1;
	for(k=0;k<10;k++);

	UP_DAT=1;
	for(k=0;k<10;k++);
	UP_DAT=0;
	delay_ms(1);
}         
//=====================================================================

//=======================ad9910发送ramdata程序=========================
void Txramdata(void)
{
	uint m,k;

	CSB=0;
	txd_8bit(0x16);    //发送ram控制字地址
	for (m=0; m<4096; m++)
		txd_8bit(ramdata_Square[m]); 
	CSB=1;
	for(k=0;k<10;k++);

	UP_DAT=1;
	for(k=0;k<10;k++);
	UP_DAT=0;
	delay_ms(1);
}         
//=====================================================================

//=======================方波参数设置和发送程序========================
void Square_wave(uint Sample_interval)//方波
{
	ulong Temp;
	Temp = Sample_interval/4; //1GHz/4, 采样间隔范围：4*(1~65536)ns
	if(Temp > 0xffff)
		Temp = 0xffff;
	ramprofile0[7] = 0x24;
	ramprofile0[6] = 0x00;
	ramprofile0[5] = 0x00;
	ramprofile0[4] = 0xc0;
	ramprofile0[3] = 0x0f;
	ramprofile0[2] = (uchar)Temp;
	ramprofile0[1] = (uchar)(Temp>>8);
	ramprofile0[0] = 0x00;
	Txramprofile();

	Txramdata();	
 }
//=====================================================================
//			delay_ms(5);
//			cfr1[0] = 0x00; //RAM 失能
//			cfr2[1] = 0x00; //DRG 失能
//			Txcfr(); //发送cfrx控制字
//			Write_Amplitude(500); //写幅度，输入范围：1-650 mV
//			Freq_convert(100000); //写频率，输入范围：1-400 000 000Hz
//		  delay_ms(5000);
//		

//			delay_ms(5);
//			cfr1[0] = 0x00; //RAM 失能
//			cfr2[1]=0x0e; //DRG 使能
//			Txcfr(); //发送cfrx控制字
//			Write_Amplitude(500); //写幅度，输入范围：1-650 mV
//			//扫频波下限频率，上限频率，频率步进（单位：Hz），步进时间间隔（单位：us）
//			SweepFre(100, 100000, 10, 120000); //步进时间范围：4*(1~65536)ns
//		  delay_ms(5000);
		

//			delay_ms(5);
//			Square_wave(200); //方波，采样时间间隔输入范围：4*(1~65536)ns
//			cfr1[0] = 0xc0; //RAM 使能，幅度控制
//			cfr2[1] = 0x00; //DRG 失能
//			Txcfr(); //发送cfrx控制字
//		  delay_ms(5000);
