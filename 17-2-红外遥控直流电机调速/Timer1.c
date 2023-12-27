#include <REGX52.H>

/**
 * @brief  ��ʱ��1��ʼ�� 100us@12.000MHz
 * @param  void
 * @retval void
 */
void Timer1_Init(void)		//100us@12.000MHz
{
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x10;		//���ö�ʱ��ģʽ
	TL1 = 0x9C;		//���ö�ʱ��ֵ
	TH1 = 0xFF;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1; 	// ����timer_1�ж�
	EA = 1; 	// ���������ж�
	PT1 = 0; 	// ����timer_1���ȼ�Ϊ0
}

/* 
// timer1 �жϷ������ģ�壨�жϺţ�3��
void Timer0_Routine() interrupt 3 {
	static unsigned int T1Count;
	// ��ʩT0Countÿ��1ms��1
	TL1 = 0x9C;		//���ö�ʱ��ֵ
	TH1 = 0xFF;		//���ö�ʱ��ֵ
	++T1Count;
	if (T1Count >= 1000) {
		T1Count = 0;
	}	
}
*/