#include "ad7606.h"

u16	ad_num;	//AD读取个数计数
int ad7606_data[8][Ndot];	//存储AD读取数据

void AD7606_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure ; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);	//使能PA,Pb,PC,PE,PG时钟
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOF, &GPIO_InitStructure);  	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);    	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);  //F口 低8位d0~d7
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);   	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);    	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //G口 高8位d8~d15
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //normal GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);	//PF8 BUSY 状态线 读
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PA1 CS信号线 写
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2 RD信号线 写
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PA3 CONVSTAB信号线 写
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //PA4 OS2 默认为1
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PA5 osc1 默认为1
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	//下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //PA6 reset 上升沿50ns
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //PF13 osc0 默认为0 OSC[2:0]=110默认64倍过采样
}

void AD7606_Init(void)
{
	AD7606_GPIO_Init();
	
	OS2 = 0;
	OS1 = 0;
	OS0 = 0;	//OSC[2:0]=001为2倍过采样
	COV = 1;	//转换开始输入AB
	CS = 1;	//片选，输出总线失能
	RST = 0;	//初始复位低电平
	RST = 0;	//延时
	RST = 1;	//复位
	RST = 0;	//初始化
}

void AD7606_read(void)
{
	int i;
	CS = 0;	//数据帧传输使能
	
	for(i = 0; i < 8; ++i)
	{
		RD = 0;	//下降沿
		RD = 0;	//延时
		RD = 0;	//延时
		RD = 0;	//延时
		RD = 1;	//上升沿
		
		ad7606_data[i][ad_num] = (GPIOF->IDR&0x00ff) | ((GPIOG->IDR&0x00ff)<<8);	//读取数据
		
		if(ad7606_data[i][ad_num] > 32767)
		{
			ad7606_data[i][ad_num] = ad7606_data[i][ad_num] - 65536;
		} 
		
	}
	
	CS = 1;	//数据帧传输失能
}
