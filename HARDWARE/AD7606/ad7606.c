#include "ad7606.h"

u16	ad_num;	//AD��ȡ��������
int ad7606_data[8][Ndot];	//�洢AD��ȡ����

void AD7606_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure ; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);	//ʹ��PA,Pb,PC,PE,PGʱ��
	

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
	GPIO_Init(GPIOF, &GPIO_InitStructure);  //F�� ��8λd0~d7
	
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
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //G�� ��8λd8~d15
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //normal GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);	//PF8 BUSY ״̬�� ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PA1 CS�ź��� д
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2 RD�ź��� д
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PA3 CONVSTAB�ź��� д
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //PA4 OS2 Ĭ��Ϊ1
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PA5 osc1 Ĭ��Ϊ1
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //PA6 reset ������50ns
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //PF13 osc0 Ĭ��Ϊ0 OSC[2:0]=110Ĭ��64��������
}

void AD7606_Init(void)
{
	AD7606_GPIO_Init();
	
	OS2 = 0;
	OS1 = 0;
	OS0 = 0;	//OSC[2:0]=001Ϊ2��������
	COV = 1;	//ת����ʼ����AB
	CS = 1;	//Ƭѡ���������ʧ��
	RST = 0;	//��ʼ��λ�͵�ƽ
	RST = 0;	//��ʱ
	RST = 1;	//��λ
	RST = 0;	//��ʼ��
}

void AD7606_read(void)
{
	int i;
	CS = 0;	//����֡����ʹ��
	
	for(i = 0; i < 8; ++i)
	{
		RD = 0;	//�½���
		RD = 0;	//��ʱ
		RD = 0;	//��ʱ
		RD = 0;	//��ʱ
		RD = 1;	//������
		
		ad7606_data[i][ad_num] = (GPIOF->IDR&0x00ff) | ((GPIOG->IDR&0x00ff)<<8);	//��ȡ����
		
		if(ad7606_data[i][ad_num] > 32767)
		{
			ad7606_data[i][ad_num] = ad7606_data[i][ad_num] - 65536;
		} 
		
	}
	
	CS = 1;	//����֡����ʧ��
}
