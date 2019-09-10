#include "key.h" 

//按键初始化函数
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; //KEY1 KEY2对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE3,4

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP对应引脚PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA0
}

//按键扫描函数
u8 KEY_Scan(void)
{
	static u8 key_up=1; //按键按松开标志	  
	
	if(key_up && (KEY0==0 || KEY1==0 || WK_UP==1))
	{
		delay_ms(2);	//去抖动 
		
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
 	return 0;	// 无按键按下
}

u8 KEY0_Scan(void)
{
	static char key_up = 0;	//按键按松开标志

	if (KEY0 == 0)
	{
		/*去抖动 */
		delay_ms(10);

		if (KEY0 == 0)
		{
			key_up = 1;
		}
		while(!KEY0);	//检测按键是否松开
	}
	
	if(key_up == 1)
	{
		key_up = 0;
		return 1;	//有按键按下
	}
	
	return 0;	//无按键按下
}
