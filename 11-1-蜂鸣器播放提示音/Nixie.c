#include <REGX52.H>
#include "Delay.h"

// ����ܶ�Ӧ���ֶ�Ӧ�Ķ����
unsigned char NixieTable[] = \
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
// 0     1     2     3     4     5     6     7     8     9

/*
 * �������������ʾ
 * ����1��Location: ָ����ʾ�������
 * ����2��Number: ָ���������ʾ������
 */
void Nixie(unsigned char Location, unsigned char Number) {
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

//	// ��̬��ʾʱע�ͣ���̬��ʾʱ��
//	// �������ܶ�̬��ʾ��λ���⡪����Ӱ 
//	Delay(1); // ά��ѡ�в���ʾ�������Ӿ�����
//	P0 = 0x00; // ��������
}