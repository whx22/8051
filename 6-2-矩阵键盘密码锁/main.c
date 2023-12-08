#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"

unsigned char KeyNum;

/**
 * ����S1-S10������1-9��0
 * ����S11��ȷ����������������������ȷ���ж�
 * ����S12����������
 */
#define PASSWORD 2345
unsigned char KeyNum;	// ��������
unsigned int password;	// ��������
unsigned int count;		// ��������λ��
void main() {
	LCD_Init();
	LCD_ShowString(1, 1, "Password:");
	while (1) {
		KeyNum = MatrixKey();
		if (KeyNum) {
			// S1-S10
			if (KeyNum <= 10) {
				if (count < 4) {
					password *= 10;
					password += KeyNum % 10;
					++count;
				}
				LCD_ShowNum(2, 1, password, 4);
			}
			// S11
			if (KeyNum == 11) {
				if (password == PASSWORD) {
					LCD_ShowString(1, 14, "OK ");
				} else {
					LCD_ShowString(1, 14, "ERR");
				}
				password = 0;
				count = 0;
				LCD_ShowNum(2, 1, password, 4);
			}
			// S12
			if (KeyNum == 12) {
				password = 0;
				count = 0;
				LCD_ShowNum(2, 1, password, 4);
			}
		}
	}
}