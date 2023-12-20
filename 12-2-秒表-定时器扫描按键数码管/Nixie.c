#include <REGX52.H>
#include "Delay.h"

// �������ʾ��������
unsigned char Nixie_Buf[9] = {0, 10, 10, 10, 10, 10, 10, 10, 10};

// ����ܶ�Ӧ���ֶ�Ӧ�Ķ����
unsigned char NixieTable[] = \
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x40};
// 0     1     2     3     4     5     6     7     8     9	  ��  	 -


/**
 * @brief 	��ָ��λ����Ϊ��Ҫ��ʾ������
 * @param	Location: ָ����ʾ�������
 * @param	Number: ָ���������ʾ������
 * @retval	void
 * @caller	main
 * @callee 	null
 */
void Nixie_SetBuf(unsigned char Location, Number) {
	Nixie_Buf[Location] = Number;	
}

/**
 * @brief 	ָ�������������ʾָ������
 * @param	Location: ָ����ʾ�������
 * @param	Number: ָ���������ʾ������
 * @retval	void
 * @caller  Nixie_Loop
 * @callee	null
 */
void Nixie_Scan(unsigned char Location, Number) {
	P0 = 0x00; // ��������
	// ���ݾ���Location������ѡ�ж�Ӧ�����
	switch(Location) {
		case 1: P2_4 = 1, P2_3 = 1, P2_2 = 1; break; // ��һ������ܣ�LED8��Y7 
		case 2: P2_4 = 1, P2_3 = 1, P2_2 = 0; break; 
		case 3: P2_4 = 1, P2_3 = 0, P2_2 = 1; break;
		case 4: P2_4 = 1, P2_3 = 0, P2_2 = 0; break;
		case 5: P2_4 = 0, P2_3 = 1, P2_2 = 1; break;
		case 6: P2_4 = 0, P2_3 = 1, P2_2 = 0; break;
		case 7: P2_4 = 0, P2_3 = 0, P2_2 = 1; break;
		case 8: P2_4 = 0, P2_3 = 0, P2_2 = 0; break;
	}
	// ����Number������ʾ��Ӧ����
	P0 = NixieTable[Number];
}

/**
 * @brief 	����ܵ�ÿһλ��ʾ��Ӧλ������
 * @param	void
 * @retval	void
 * @caller 	Timer0_Routine interrupt 1
 * @callee	Nixie_Scan
 * @note	Timer0_Routine��ÿ���2ms����һ�θú�����
 			ʵ������ܶ�̬ˢ����ʾ������������cpuռ��ָ��Ӱ��������ʾ
 */
void Nixie_Loop(void) {
	static unsigned char i = 1 ;
	Nixie_Scan(i, Nixie_Buf[i]);
	++i;
	if (i > 8) { // ��λ��ʾ���������е�����
		i = 1;
	}
}