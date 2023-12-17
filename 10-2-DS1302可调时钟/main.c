#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Key.h"
#include "Timer0.h"

unsigned char KeyNum, MODE, TimeSetSelect, TimeSetFlashFlag;

void TimeShow(void) {
	DS1302_ReadTime();
	LCD_ShowNum(1, 1, DS1302_Time[0], 2);
	LCD_ShowNum(1, 4, DS1302_Time[1], 2);
	LCD_ShowNum(1, 7, DS1302_Time[2], 2);
	LCD_ShowNum(2, 1, DS1302_Time[3], 2);
	LCD_ShowNum(2, 4, DS1302_Time[4], 2);
	LCD_ShowNum(2, 7, DS1302_Time[5], 2);
}

void TimeSet(void) {
	if (KeyNum == 2) { // ѡ������λ
		++TimeSetSelect;
		TimeSetSelect %= 6;
	}
	if (KeyNum == 3) { // ���ö�Ӧ��������
		++DS1302_Time[TimeSetSelect];
		if (DS1302_Time[0] > 99) {DS1302_Time[0] = 0;}
		if (DS1302_Time[1] > 12) {DS1302_Time[1] = 1;}
		if (DS1302_Time[2] > 30) {DS1302_Time[2] = 1;}
		if (DS1302_Time[3] > 23) {DS1302_Time[3] = 0;}
		if (DS1302_Time[4] > 59) {DS1302_Time[4] = 0;}
		if (DS1302_Time[5] > 59) {DS1302_Time[5] = 0;}
	}
	if (KeyNum == 4) { // ���ö�Ӧ���ּ�С
		--DS1302_Time[TimeSetSelect];
		if (DS1302_Time[0] < 0) {DS1302_Time[0] = 99;}
		if (DS1302_Time[1] < 1) {DS1302_Time[1] = 12;}
		if (DS1302_Time[2] < 1) {DS1302_Time[2] = 30;}
		if (DS1302_Time[3] < 0) {DS1302_Time[3] = 23;}
		if (DS1302_Time[4] < 0) {DS1302_Time[4] = 59;}
		if (DS1302_Time[5] < 0) {DS1302_Time[5] = 59;}
	}
	// ʵ������ģʽѡ����˸
	if (TimeSetSelect == 0 && TimeSetFlashFlag == 1) {
		LCD_ShowString(1, 1, "  ");
	} else {
		LCD_ShowNum(1, 1, DS1302_Time[0], 2);
	}
	if (TimeSetSelect == 1 && TimeSetFlashFlag == 1) {
		LCD_ShowString(1, 4, "  ");
	} else {
		LCD_ShowNum(1, 4, DS1302_Time[1], 2);
	}
	if (TimeSetSelect == 2 && TimeSetFlashFlag == 1) {
		LCD_ShowString(1, 7, "  ");
	} else {
		LCD_ShowNum(1, 7, DS1302_Time[2], 2);
	}
	if (TimeSetSelect == 3 && TimeSetFlashFlag == 1) {
		LCD_ShowString(2, 1, "  ");
	} else {
		LCD_ShowNum(2, 1, DS1302_Time[3], 2);
	}
	if (TimeSetSelect == 4 && TimeSetFlashFlag == 1) {
		LCD_ShowString(2, 4, "  ");
	} else {
		LCD_ShowNum(2, 4, DS1302_Time[4], 2);
	}
	if (TimeSetSelect == 5 && TimeSetFlashFlag == 1) {
		LCD_ShowString(2, 7, "  ");
	} else {
		LCD_ShowNum(2, 7, DS1302_Time[5], 2);
	}
	//LCD_ShowNum(2, 10, TimeSetSelect, 2); // (debug)��ʾ��ǰ����λ
	//LCD_ShowNum(2, 13, TimeSetFlashFlag, 2); // (debug)��ʾ��ǰ��˸��־ 
}

void main() {
	LCD_Init();
	DS1302_Init();
	Timer0Init();
	LCD_ShowString(1, 1, "  -  -  ");
	LCD_ShowString(2, 1, "  :  :  ");	

//	DS1302_WriteByte(0x8E, 0x00); // ���DS1302д����
														  
	DS1302_SetTime();
	while (1) {
		KeyNum = Key();
		if (KeyNum == 1) { // ģʽ�л�
			if (MODE == 0) { // ��ʾģʽ -> ����ģʽ
				MODE = 1;
				TimeSetSelect = 0; // ��������ģʽʱ������λ������
			} else if (MODE == 1) { // ����ģʽ -> ��ʾģʽ 
				MODE = 0;
				DS1302_SetTime(); // �˳�����ģʽʱ���������� 
			}
		}
		switch (MODE) {
			case 0: TimeShow(); break; // ��ʾģʽ
			case 1: TimeSet(); break;  // ����ģʽ
		}
	}
}

void Timer0_Routine() interrupt 1 {
	static unsigned int T0Count;
	// ��ʩT0Countÿ��1ms��1
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	++T0Count;
	if (T0Count >= 500) {
		T0Count = 0;
		TimeSetFlashFlag = !TimeSetFlashFlag;
	}	
}