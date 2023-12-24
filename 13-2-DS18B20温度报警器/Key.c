#include <REGX52.h>
#include "Delay.h"

unsigned char Key_KeyNumber;

/**
 * @brief 	���ذ��µļ���ֵ��������ȫ�ֱ���Key_KeyNumber
 * @param	void
 * @retval 	���µİ�������
 * @caller	main
 * @callee	null
 */
unsigned char Key(void) {
	// ʵ�ְ���ֻ����һ�Σ�����0��ʹ��Temp���棬������ȫ�ֱ���Key_KeyNumber
	unsigned char Temp = 0;
	Temp = Key_KeyNumber;
	Key_KeyNumber = 0;
	return Temp;
}

/**
 * @brief 	��ȡ����״̬����
 * @param	void
 * @retval	����״̬
 * @caller	Key_Loop
 * @callee	null
 * @note	�������£�������Ӧ�˿�Ϊ�͵�ƽ
 */
unsigned char Key_GetState(void) {
	unsigned char KeyNumber = 0;
	if (P3_1 == 0) {KeyNumber = 1;}
	if (P3_0 == 0) {KeyNumber = 2;}
	if (P3_2 == 0) {KeyNumber = 3;}
	if (P3_3 == 0) {KeyNumber = 4;}
	return KeyNumber;
}

/**
 * @brief 	����ɨ�裬��ȫ�ֱ���Key_GetState����Ϊ��ȡ���ļ���
 * @param	void
 * @retval	void
 * @caller	Timer0_Routine interrupt 1
 * @callee	Key_GetState
 * @note 	ͨ���жϰ���״̬�Ƿ���ڴӰ��µ����ֵ�״̬ת����ʵ������ʱ��Ӧ
 			Timer0_Routine��ÿ���20ms����һ�θú�����
			ʵ�ֲ���ɨ�谴�������������������º��ɿ��ķ���
 */
void Key_Loop(void) {
	static unsigned char NowState, LastState;
	LastState = NowState;
	NowState = Key_GetState();
	if (LastState == 1 && NowState == 0) {
		Key_KeyNumber = 1;
	}
	if (LastState == 2 && NowState == 0) {
		Key_KeyNumber = 2;
	}
	if (LastState == 3 && NowState == 0) {
		Key_KeyNumber = 3;
	}
	if (LastState == 4 && NowState == 0) {
		Key_KeyNumber = 4;
	}	
}