#include "display.h"

float fir_fs[Total_Ndot];	//�������˲������
extern float time_fs[4][Ndot]; //���ڿ����������ʱ������
extern int ad7606_data[8][Ndot];
extern float fsl, fsh;
float Riout,Roout,Apvout,flsout,fhsout;

/*
��׼����ʽ��
(y - min)/(max - min) = (Y - MIN)/(MAX - MIN)
Y = [(MAX - MIN)/(max - min )]*y + [MIN - (MAX - MIN)/(max - min )*min] = k*y + b
��	k = (MAX - MIN)/(max - min)
	b = MIN - (MAX - MIN)/(max - min )*min = MIN - k*min
	
����min > 0���Σ���׼�����ֵΪ2^12 = 32767����Сֵ�ɲ��α�ƽ�̶�A = (max - min)/MAX = (max - min)/32767ȷ��
				��MIN = A > 1 ? 16383/(A - 1) : 16383*(1 - A)
				
����max < 0���Σ���׼����СֵΪ0�����ֵ�ɲ��α�ƽ�̶�A = (max - min)/MAX = (max - min)/32767ȷ��
				��MAX = A > 1 ? 16383*(1-1/A) : 16383*(1 - A)
*/

void Display_Init(void)
{
	LCD_ShowString(20, 120, 200, 4, 24, (u8*)"Spectrum Analysis");	//����
	LCD_ShowString(65, 160, 200, 4, 24, (u8*)"Welcome!");	//����
}

void display_param_form(void)
{
	LCD_Fill(0, 0, 240, 320, WHITE);
	
	LCD_ShowString(15, 22, 230, 4, 16, (u8*)"circuit characteristic test");	
	LCD_ShowString(85, 52, 220, 4, 16, (u8*)"Apv=");
	LCD_ShowString(25, 82, 220, 4, 16, (u8*)"Ri =");
	LCD_ShowString(130, 82, 220, 4, 16,(u8*)"Ro =");
	LCD_ShowString(25, 112, 220, 4, 16,(u8*)"fls=");
	LCD_ShowString(130, 112, 220, 4, 16,(u8*)"fhs=");
	
	LCD_DrawLine(5,5 ,5, 315);//��ͼ
	LCD_DrawLine(5,5 ,235, 5);
	LCD_DrawLine(235,315 ,235, 5);
	LCD_DrawLine(235,315 ,5, 315);

	LCD_DrawLine(5,40 ,235, 40);  //����
	LCD_DrawLine(5,70 ,235, 70);
	LCD_DrawLine(5,100 ,235, 100);
	LCD_DrawLine(5,130 ,235,130);
	
	LCD_DrawLine(120,70 ,120,130);//����
	
	
	
	LCD_DrawLine(20,135 ,20, 300);//����ϵy��
	LCD_DrawLine(20,135 ,15, 140);
	LCD_DrawLine(20,135 ,25, 140);
	LCD_ShowString(7, 140, 220, 4, 12, (u8*)"dB");
	
	LCD_DrawLine(20,300,225, 300);//����ϵx��
	LCD_DrawLine(220,295,225, 300);
	LCD_DrawLine(220,305,225, 300);
	LCD_ShowString(195, 302, 220, 4, 12, (u8*)"f/hz");
	
	LCD_ShowString(11, 293, 220, 4, 16, (u8*)"0");//ԭ��
}
void display_fault_form(void)
{
	LCD_Fill(0, 0, 240, 320, WHITE);
	
	LCD_ShowString(25, 14, 220, 4, 16, (u8*)"circuit fault analysis");	
	LCD_ShowString(25, 39, 220, 4, 16, (u8*)"  ");	
	LCD_ShowString(25, 64, 220, 4, 16, (u8*)"R1");
	LCD_ShowString(25, 89, 220, 4, 16, (u8*)"R2");
	LCD_ShowString(25, 114, 220, 4, 16, (u8*)"R3");
	LCD_ShowString(25, 139, 220, 4, 16, (u8*)"R4");
	LCD_ShowString(65, 164, 220, 4, 16, (u8*)"amplify");
	LCD_ShowString(160, 164, 220, 4, 16, (u8*)"break");
	LCD_ShowString(25, 189, 220, 4, 16, (u8*)"C1");
	LCD_ShowString(25, 214, 220, 4, 16, (u8*)"C2");
	LCD_ShowString(25, 239, 220, 4, 16, (u8*)"C3");
	LCD_ShowString(70, 39, 100, 4, 16, (u8*)"short");
	LCD_ShowString(160, 39, 100, 4, 16, (u8*)"berak");
	LCD_ShowString(25, 264, 230, 4, 16, (u8*)"0 is normal.");
	LCD_ShowString(25, 289, 230, 4, 16, (u8*)"1 is abnormal.");
	
	LCD_DrawLine(5,5 ,5, 315);//���ͼ
	LCD_DrawLine(5,5 ,235, 5);
	LCD_DrawLine(235,315 ,235, 5);
	LCD_DrawLine(235,315 ,5, 315);
	 
	LCD_DrawLine(5,30 ,235, 30);
	LCD_DrawLine(5,55 ,235, 55);
	LCD_DrawLine(5,80 ,235, 80);
	LCD_DrawLine(5,105 ,235,105);
	LCD_DrawLine(5,130 ,235, 130);
	LCD_DrawLine(5,155 ,235, 155);
	LCD_DrawLine(5,180 ,235, 180);
	LCD_DrawLine(5,205 ,235,205);
	LCD_DrawLine(5,230 ,235, 230);//����
	LCD_DrawLine(5,255 ,235, 255);//����

	
	LCD_DrawLine(50,30 ,50,255);   //����
	LCD_DrawLine(140,30 ,140,255);
	LCD_DrawLine(5,30,50,55);//б��
	LCD_DrawLine(5,155,50,180);//б��
}

void display_param(void)
{
	LCD_ShowNum(70, 82, Ri, 5, 16);
	LCD_ShowNum(175, 82, Ro, 5, 16);
	LCD_ShowNum(125, 52, Av, 5, 16);	
}


void display_amp_fre(void)
{
	int i=0;    //x����
	int j1=0;    //������ʾλ��
	float max;	//��ѹ���ֵ
	float min;	
	int MAX = 150;
	int MIN = 300;
	float k, b;
	
	flsout=fsl;
	fhsout=fsh;
	
	max = min = Avs[0];
	
	for(i = 1; i < 2195; ++i)
	{
		if(max < Avs[i])
		{
			max = Avs[i];
		}
		if(min > Avs[i])
		{
			min = Avs[i];
		}
	}
	
	k = (MAX - MIN)/(max - min);
	b = MIN - k*min;
	 
	LCD_Fill(21, 150, 220, 299, WHITE);
	 
	LCD_DrawLine(40,295,40, 300);//�������
	LCD_DrawLine(60,295,60, 300);
	LCD_DrawLine(80,295,80, 300);
	LCD_DrawLine(100,295,100, 300);
	LCD_DrawLine(120,295,120, 300);
	LCD_DrawLine(140,295,140, 300);
	LCD_DrawLine(160,295,160, 300);
	LCD_DrawLine(180,295,180, 300);
	LCD_DrawLine(200,295,200, 300);
	 
	LCD_DrawLine(20,280,25, 280);//�������
    LCD_DrawLine(20,260,25, 260);
	LCD_DrawLine(20,240,25, 240);
	LCD_DrawLine(20,220,25, 220);
	LCD_DrawLine(20,200,25, 200);
	LCD_DrawLine(20,180,25, 180);
	LCD_DrawLine(20,160,25, 160);
	 
	for( i = 20, j1 = 0; i < 95 ; ++i,j1+=13)
	{	
		LCD_DrawLine(i, k*Avs[j1] + b, i+1, k*Avs[j1+13]+ b);
	}
	
	for( i = 95, j1 = 1000; i < 145; ++i,j1+=20)
	{	
		LCD_DrawLine(i, k*Avs[j1] + b, i+1, k*Avs[j1+20]+ b);
	}
	
	for( i = 145, j1 = 2000; i < 220; ++i,j1+=1)
	{	
		LCD_DrawLine(i, k*Avs[j1] + b, i+1, k*Avs[j1+1]+ b);
	}
	
	LCD_ShowNum(70, 112, fsl, 5, 16);
	LCD_ShowNum(175, 112, fsh, 6, 16);	
}


void display_fault(int y)
{
	int x=y;

	if(x==1)//R1��·                                   ���ݴ�����������
	LCD_ShowString(90, 64, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(90, 64,  50, 4, 16, (u8*)"0");
	if(x==2)
	LCD_ShowString(90, 89, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(90, 89, 50, 4, 16, (u8*)"0");
	if(x==3)
	LCD_ShowString(90, 114, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(90, 114, 50, 4, 16, (u8*)"0");
	if(x==4)
	LCD_ShowString(90, 139, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(90, 139, 50, 4, 16, (u8*)"0");
	if(x==5)//R1��·
	LCD_ShowString(180, 64, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(180, 64, 50, 4, 16, (u8*)"0");
	if(x==6)
	LCD_ShowString(180, 89, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(180, 89, 50, 4, 16, (u8*)"0");
	if(x==7)
	LCD_ShowString(180, 114, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(180, 114, 50, 4, 16, (u8*)"0");
	if(x==8)
	LCD_ShowString(180, 139, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(180, 139, 50, 4, 16, (u8*)"0");
	if(x==9)//C����2��
	LCD_ShowString(90, 189, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(90, 189, 50, 4, 16, (u8*)"0");
	if(x==10)
	LCD_ShowString(90, 214, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(90, 214, 50, 4, 16, (u8*)"0");
	if(x==11)
	LCD_ShowString(90, 239, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(90, 239, 50, 4, 16, (u8*)"0");
	if(x==12)//C��·
	LCD_ShowString(180, 189, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(180, 189, 50, 4, 16, (u8*)"0");
	LCD_ShowString(180, 214, 50, 4, 16, (u8*)"0");
	LCD_ShowString(180, 239, 50, 4, 16, (u8*)"0");
	if(x==13)
	LCD_ShowString(180, 214, 50, 4, 16, (u8*)"1");
	else 	LCD_ShowString(180, 214, 50, 4, 16, (u8*)"0");
	if(x==14)
	LCD_ShowString(180, 239, 50, 4, 16, (u8*)"1");
	else LCD_ShowString(180, 239, 50, 4, 16, (u8*)"0");
}

void time_display(float *input_dot)
{
	float max;	//��ѹ���ֵ
	float min;	//��ѹ��Сֵ
	char str[10];	//�洢��ֵת�ַ����ַ�������
	int i;
	
	max = min = input_dot[0];

	for(i = 0; i < 2*dot_num/frequency; ++i)	//��һ���������������Сֵ
	{
		if(input_dot[i] > max)
		{
			max = input_dot[i];
		}
		if(input_dot[i] < min)
		{
			min = input_dot[i];
		}
	}
	
	max = max*5/32767;
	min = min*5/32767;
	
	//��ʱ��ԭ����
	LCD_Fill(0, 0, 240, 320, WHITE);
	
	LCD_ShowString(65, TOP_EDGE/4, 120, 4, 16, (u8*)"Original Signal");	//����
	
	LCD_DrawLine(POINT_X(0), 320 - 160, POINT_X(POINT_NUM + RIGHT_EDGE/2), 320 - 160);	//x��
	LCD_DrawLine(POINT_X(POINT_NUM), 320 - 160 - 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), 320 - 160);	//x���ϼ�ͷ
	LCD_DrawLine(POINT_X(POINT_NUM), 320 - 160 + 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), 320 - 160);	//x���¼�ͷ
	LCD_ShowString(POINT_X(POINT_NUM - 15), 320 - 160 + 5, 25, 5, 16, (u8*)"t/ms");	//x���������
	
	LCD_ShowString(LEFT_EDGE/2, 320 - BOTTOM_EDGE, 5, 5, 16, (u8*)"O");	//����ԭ��
	
	LCD_DrawLine(POINT_X(0), 320 - BOTTOM_EDGE, POINT_X(0), TOP_EDGE/2);	//y��
	LCD_DrawLine(POINT_X(-5), TOP_EDGE/2 + 5, POINT_X(0), TOP_EDGE/2);	//y�ᳯ���ͷ
	LCD_DrawLine(POINT_X(5), TOP_EDGE/2 + 5, POINT_X(0), TOP_EDGE/2);		//y�ᳯ�Ҽ�ͷ
	LCD_ShowString(POINT_X(5), TOP_EDGE/2 - 5, 17, 4, 16, (u8*)"U/V");	//y���������
	
	sprintf(str, "%.2f", max);
	LCD_ShowString(10, 320 - BOTTOM_EDGE/3, 25, 4, 12, (u8*)"Umax:");	//���ֵ
	LCD_ShowString(40, 320 - BOTTOM_EDGE/3, 30, 4, 12, (u8*)str);
	LCD_ShowString(70, 320 - BOTTOM_EDGE/3, 5, 4, 12, (u8*)"V");
	
	sprintf(str, "%.2f", min);
	LCD_ShowString(95, 320 - BOTTOM_EDGE/3, 25, 4, 12, (u8*)"Umin:");	//��Сֵ
	LCD_ShowString(125, 320 - BOTTOM_EDGE/3, 30, 4, 12, (u8*)str);
	LCD_ShowString(155, 320 - BOTTOM_EDGE/3, 5, 4, 12, (u8*)"V");
	
	sprintf(str, "%.1e", (double)frequency);
	LCD_ShowString(180, 320 - BOTTOM_EDGE/3, 10, 4, 12, (u8*)"f:");	//Ƶ��
	LCD_ShowString(190, 320 - BOTTOM_EDGE/3, 20, 4, 12, (u8*)str);
	LCD_ShowString(215, 320 - BOTTOM_EDGE/3, 10, 4, 12, (u8*)&str[6]);
	LCD_ShowString(225, 320 - BOTTOM_EDGE/3, 25, 4, 12, (u8*)"Hz");
	
	for(int i = 0; i < POINT_NUM; ++i)
	{
		LCD_DrawLine(POINT_X(i), POINT_Y(input_dot[i]) - 160 + BOTTOM_EDGE, POINT_X(i+1), POINT_Y(input_dot[i+1]) - 160 + BOTTOM_EDGE);
	}
}

void fir_display(float *input_dot)
{
	float k;	//��׼��ϵ��
	float b;	//��׼���ؾ�
	float max;	//�˲������ֵ
	float min;	//�˲�����Сֵ
	char str[10];	//�洢��ֵת�ַ����ַ�������
	int i;
	
	for(i = 0; i < Total_Ndot; ++i)
	{
		fir_fs[i] = fir(input_dot[i]);	//�Է��������˲�
	}
	
	max = min = fir_fs[FIR_ORDER];

	for(i = FIR_ORDER; i < FIR_ORDER + dot_num/frequency; ++i)	//��һ���������������Сֵ
	{
		if(fir_fs[i] > max)
		{
			max = fir_fs[i];
		}
		if(fir_fs[i] < min)
		{
			min = fir_fs[i];
		}
	}
			
	//��ʱ���˲�ͼ
	LCD_Fill(0, 0, 240, 320, WHITE);
	
	LCD_ShowString(60, TOP_EDGE/4, 125, 4, 16, (u8*)"Filtering Signal");	//����
	
	LCD_DrawLine(POINT_X(0), 320 - BOTTOM_EDGE, POINT_X(0), TOP_EDGE/2);	//y��
	LCD_DrawLine(POINT_X(-5), TOP_EDGE/2 + 5, POINT_X(0), TOP_EDGE/2);	//y�ᳯ���ͷ
	LCD_DrawLine(POINT_X(5), TOP_EDGE/2 + 5, POINT_X(0), TOP_EDGE/2);		//y�ᳯ�Ҽ�ͷ
	LCD_ShowString(POINT_X(5), TOP_EDGE/2 - 5, 17, 4, 16, (u8*)"U/V");	//y���������
	
	if(max*min <= 0)	//���ο�x��
	{
		k = (32767 - 0)/(max - min);
		b = 0 - k*min;
		
		LCD_ShowString(LEFT_EDGE/2, POINT_Y(b), 5, 5, 16, (u8*)"O");	//����ԭ��
		LCD_DrawLine(POINT_X(0), POINT_Y(b), POINT_X(POINT_NUM + RIGHT_EDGE/2), POINT_Y(b));	//x��
		LCD_DrawLine(POINT_X(POINT_NUM), POINT_Y(b) - 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), POINT_Y(b));	//x���ϼ�ͷ
		LCD_DrawLine(POINT_X(POINT_NUM), POINT_Y(b) + 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), POINT_Y(b));	//x���¼�ͷ
		LCD_ShowString(POINT_X(POINT_NUM - 15), POINT_Y(b) + 5, 25, 5, 16, (u8*)"t/ms");	//x���������
	}
	
	if(min > 0)	//������x���Ϸ�
	{
		float A = (max - min)/32767;
		int MIN = A > 1 ? 16383/(A - 1) : 16383*(1 - A);
		k = (32767 - MIN)/(max - min);
		b = MIN - k*min;
		
		LCD_ShowString(LEFT_EDGE/2, 320 - BOTTOM_EDGE, 5, 5, 16, (u8*)"O");	//����ԭ��
		LCD_DrawLine(POINT_X(0), 320 - BOTTOM_EDGE, POINT_X(POINT_NUM + RIGHT_EDGE/2), 320 - BOTTOM_EDGE);	//x��
		LCD_DrawLine(POINT_X(POINT_NUM), 320 - BOTTOM_EDGE - 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), 320 - BOTTOM_EDGE);	//x���ϼ�ͷ
		LCD_DrawLine(POINT_X(POINT_NUM), 320 - BOTTOM_EDGE + 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), 320 - BOTTOM_EDGE);	//x���¼�ͷ
		LCD_ShowString(POINT_X(POINT_NUM - 15), 320 - BOTTOM_EDGE + 5, 25, 5, 16, (u8*)"t/ms");	//x���������
	}
	
	if(max < 0)	//������x���·�
	{
		float A = (max - min)/32767;
		int MAX = A > 1 ? 16383*(1-1/A) : 16383*(1 - A);
		k = (3584 - 0)/(max - min);
		b = 0 - k*min;
		
		LCD_ShowString(LEFT_EDGE/2, TOP_EDGE, 5, 5, 16, (u8*)"O");	//����ԭ��
		LCD_DrawLine(POINT_X(0), TOP_EDGE, POINT_X(POINT_NUM + RIGHT_EDGE/2), TOP_EDGE);	//x��
		LCD_DrawLine(POINT_X(POINT_NUM), TOP_EDGE - 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), TOP_EDGE);	//x���ϼ�ͷ
		LCD_DrawLine(POINT_X(POINT_NUM), TOP_EDGE + 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), TOP_EDGE);	//x���¼�ͷ
		LCD_ShowString(POINT_X(POINT_NUM - 15), TOP_EDGE - 5, 25, 5, 16, (u8*)"t/ms");	//x���������
	}
	
	max = max*5/32767;
	min = min*5/32767;
	
	sprintf(str, "%.2f", max);
	LCD_ShowString(10, 320 - BOTTOM_EDGE/3, 25, 4, 12, (u8*)"Umax:");	//���ֵ
	LCD_ShowString(40, 320 - BOTTOM_EDGE/3, 30, 4, 12, (u8*)str);
	LCD_ShowString(70, 320 - BOTTOM_EDGE/3, 5, 4, 12, (u8*)"V");
	
	sprintf(str, "%.2f", min);
	LCD_ShowString(95, 320 - BOTTOM_EDGE/3, 25, 4, 12, (u8*)"Umin:");	//��Сֵ
	LCD_ShowString(125, 320 - BOTTOM_EDGE/3, 30, 4, 12, (u8*)str);
	LCD_ShowString(155, 320 - BOTTOM_EDGE/3, 5, 4, 12, (u8*)"V");
	
	sprintf(str, "%.1e", (double)frequency);
	LCD_ShowString(180, 320 - BOTTOM_EDGE/3, 10, 4, 12, (u8*)"f:");	//Ƶ��
	LCD_ShowString(190, 320 - BOTTOM_EDGE/3, 20, 4, 12, (u8*)str);
	LCD_ShowString(215, 320 - BOTTOM_EDGE/3, 10, 4, 12, (u8*)&str[6]);
	LCD_ShowString(225, 320 - BOTTOM_EDGE/3, 25, 4, 12, (u8*)"Hz");
	
	for(int i = 0; i < POINT_NUM; ++i)	//���˲�ͼ����
	{
		LCD_DrawLine(POINT_X(i), POINT_Y(k*fir_fs[i+FIR_ORDER] + b), POINT_X(i+1), POINT_Y(k*fir_fs[i+FIR_ORDER+1] + b));
	}
}

void freq_display(float *input_dot)
{
	char str[10];	//�洢��ֵת�ַ����ַ�������
	float value;	//Ƶ��ͼÿ�㴦��ֵ
	u8 line_num;	//���߸���
	
	float_fft(input_dot, 6);	//���ٸ���Ҷ�仯
	//��ԭ����Ƶ����
	LCD_Fill(0, 0, 240, 320, WHITE);
	
	LCD_ShowString(55, TOP_EDGE/4, 140, 4, 16, (u8*)"Amplitude Spectrum");	//����
	
	LCD_ShowString(LEFT_EDGE/2, 320 - BOTTOM_EDGE, 5, 5, 16, (u8*)"O");	//����ԭ��
	LCD_DrawLine(POINT_X(0), 320 - BOTTOM_EDGE, POINT_X(POINT_NUM + RIGHT_EDGE/2), 320 - BOTTOM_EDGE);	//x��
	LCD_DrawLine(POINT_X(POINT_NUM), 320 - BOTTOM_EDGE - 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), 320 - BOTTOM_EDGE);	//x���ϼ�ͷ
	LCD_DrawLine(POINT_X(POINT_NUM), 320 - BOTTOM_EDGE + 5, POINT_X(POINT_NUM + RIGHT_EDGE/2), 320 - BOTTOM_EDGE);	//x���¼�ͷ
	LCD_ShowString(POINT_X(POINT_NUM - 15), 320 - BOTTOM_EDGE - 28, 25, 5, 16, (u8*)"f/Hz");	//x���������
	
	LCD_DrawLine(POINT_X(0), 320 - BOTTOM_EDGE, POINT_X(0), TOP_EDGE/2);	//y��
	LCD_DrawLine(POINT_X(-5), TOP_EDGE/2 + 5, POINT_X(0), TOP_EDGE/2);	//y�ᳯ���ͷ
	LCD_DrawLine(POINT_X(5), TOP_EDGE/2 + 5, POINT_X(0), TOP_EDGE/2);		//y�ᳯ�Ҽ�ͷ
	LCD_ShowString(POINT_X(5), TOP_EDGE/2 - 5, 10, 4, 16, (u8*)"An");	//y���������
	
	line_num = Ndot/2 > POINT_NUM ? POINT_NUM : Ndot/2;
	
	for(int i = 0; i < line_num; ++i)
	{
		
		value = vk[i]*200;
		
		if(i == 0)
		{
			value /= 2;
		}
		
		LCD_DrawLine(POINT_X(i), POINT_Y(0), POINT_X(i), POINT_Y(value));
		
		if(i == frequency/(dot_num/Ndot))
		{
			sprintf(str, "%.0f", i*(double)dot_num/Ndot);
			LCD_ShowString(POINT_X(i-5), 320 - 0.9*BOTTOM_EDGE, 30, 4, 12, (u8*)str);	//x���ע
			sprintf(str, "%.2f", vk[i]);
			//LCD_ShowString(POINT_X(i+5), POINT_Y(value + 30), 30, 4, 12, (u8*)str);	//y���ע
			LCD_ShowString(125, 320 - BOTTOM_EDGE/3, 50, 4, 12, (u8*)str);
		}
	}
}
