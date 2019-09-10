#include "key.h" 

//������ʼ������
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; //KEY1 KEY2��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE3,4

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA0
}

//����ɨ�躯��
u8 KEY_Scan(void)
{
	static u8 key_up=1; //�������ɿ���־	  
	
	if(key_up && (KEY0==0 || KEY1==0 || WK_UP==1))
	{
		delay_ms(2);	//ȥ���� 
		
		key_up = 0;
		if(WK_UP==1)
		{
			return 1;
		}
		else if(KEY0==0)
		{
			return 2;
		}
		else if(KEY1==0)
		{
			return 3;
		}
	}
	else if(KEY0==1 && KEY1==1 && WK_UP==0)
	{
		key_up = 1; 
	}	    
 	return 0;	// �ް�������
}

u8 KEY0_Scan(void)
{
	static char key_up = 0;	//�������ɿ���־

	if (KEY0 == 0)
	{
		/*ȥ���� */
		delay_ms(10);

		if (KEY0 == 0)
		{
			key_up = 1;
		}
		while(!KEY0);	//��ⰴ���Ƿ��ɿ�
	}
	
	if(key_up == 1)
	{
		key_up = 0;
		return 1;	//�а�������
	}
	
	return 0;	//�ް�������
}
