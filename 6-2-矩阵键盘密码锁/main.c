#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"

unsigned char KeyNum;

/**
 * 按键S1-S10：数字1-9、0
 * 按键S11：确认密码键，并对密码进行正确性判断
 * 按键S12：清空输入键
 */
#define PASSWORD 2345
unsigned char KeyNum;	// 按键输入
unsigned int password;	// 输入密码
unsigned int count;		// 输入密码位数
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