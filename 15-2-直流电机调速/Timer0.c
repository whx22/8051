#include <REGX52.H>

/**
 * @brief  ��ʱ��0��ʼ�� 100us@12.000MHz
 * @param  void
 * @retval void
 */
void Timer0_Init(void)		//100us@12.000MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x9C;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1; 	// ����timer_0�ж�
	EA = 1; 	// ���������ж�
	PT0 = 0; 	// ����timer_0���ȼ�Ϊ0
}

/* 
// timer0 �жϷ������ģ�壨�жϺţ�1��
void Timer0_Routine() interrupt 1 {
	static unsigned int T0Count;
	// ��ʩT0Countÿ��1ms��1
	TL0 = 0x9C;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
	++T0Count;
	if (T0Count >= 1000) {
		T0Count = 0;
	}	
}
*/