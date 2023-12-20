#include <REGX52.H>
#include "Timer0.h"
#include "Key.h"
#include "Nixie.h"
#include "Delay.h"
#include "AT24C02.h"

unsigned char KeyNum; // �����µİ�����ֵ
unsigned char Min, Sec, MiniSec; // ���ʱ������
unsigned RunFlag; // ������б�־

void main() {
	Timer0_Init();
	while (1) {
		KeyNum = Key();
		if (KeyNum == 1) { // �����״̬ת��
			RunFlag = !RunFlag;
		}
		if (KeyNum == 2) { // �������
			Min = 0;
			Sec = 0;
			MiniSec = 0;
		}
		if (KeyNum == 3) { // ����ʱд��AT24C02
			AT24C02_WriteByte(0, Min);
			Delay(5);
			AT24C02_WriteByte(1, Sec);
			Delay(5);
			AT24C02_WriteByte(2, MiniSec);
			Delay(5);
		}
		if (KeyNum == 4) { // ��AT24C02����������Ϊ����ʱ
		  	Min = AT24C02_ReadByte(0);
			Sec = AT24C02_ReadByte(1);
			MiniSec = AT24C02_ReadByte(2);
		}
		// �������ʾ����
		Nixie_SetBuf(1, Min / 10);
		Nixie_SetBuf(2, Min % 10);
		Nixie_SetBuf(3, 11);
		Nixie_SetBuf(4, Sec / 10);
		Nixie_SetBuf(5, Sec % 10);
		Nixie_SetBuf(6, 11);
		Nixie_SetBuf(7, MiniSec / 10);
		Nixie_SetBuf(8, MiniSec % 10);
	}
}

/**
 * @brief 	����ʱ
 * @param	void
 * @retval  void
 * @caller	Timer0_Routine() interrupt 1
 * @callee  null
 * @note 	Timer0_Routine��ÿ���10ms����һ�θú�����
 			���������������ģʽ
 */
void Second_Loop(void) {
	if (RunFlag) {
	  	++MiniSec;
		if (MiniSec > 99) {
			MiniSec = 0;
			++Sec;
			if (Sec > 59) {
				Sec = 0;
				++Min;
				if (Min > 59) {
					Min = 0;
				}
			}
		}
	}
} 

/**
 * @brief 	��ʱ��0�жϴ�����	
 * @param	void
 * @retval	void
 * @caller 	hardware
 * @callee 	null
 */
void Timer0_Routine(void) interrupt 1 {
	static unsigned int T0Count1, T0Count2, T0Count3;
	// ��ʩT0Countÿ��1ms��1
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	
	// �ж�ʵ�ְ���ɨ��
	++T0Count1;
	if (T0Count1 >= 20) {
		T0Count1 = 0;
		Key_Loop();
	}

	// �ж�ʵ�������ɨ��
	++T0Count2;
	if (T0Count2 >= 2) {
		T0Count2 = 0;
		Nixie_Loop();
	}

	// �ж�ʵ��ʱ�����
	++T0Count3;
	if (T0Count3 >= 10) {
		T0Count3 = 0;
		Second_Loop();
	}
}