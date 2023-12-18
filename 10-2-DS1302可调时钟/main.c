#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Key.h"
#include "Timer0.h"

unsigned char KeyNum, MODE, TimeSetSelect, TimeSetFlashFlag;

/**
 * @brief 	对于给定的年、月，返回其对应的天数
 * @param1 	年
 * @param2	月
 * @retval	其对应的天数
 */
char days_of_month(char year, month) {
	char days_arrays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
		days_arrays[1] = 29;
	}
	return days_arrays[month - 1];
}

/**
 * @breif 	显示模式工作函数
 			读取DS1302中的日期数据到单片机数据区（RAM）
 			并使用LCD1602显示日期数据
 * @param	无
 * @retval 	无
 */
void TimeShow(void) {
	DS1302_ReadTime();
	LCD_ShowNum(1, 1, DS1302_Time[0], 2);
	LCD_ShowNum(1, 4, DS1302_Time[1], 2);
	LCD_ShowNum(1, 7, DS1302_Time[2], 2);
	LCD_ShowNum(2, 1, DS1302_Time[3], 2);
	LCD_ShowNum(2, 4, DS1302_Time[4], 2);
	LCD_ShowNum(2, 7, DS1302_Time[5], 2);
}

/**
 * @breif 	设置模式工作函数
 			设置单片机数据区（EEPROM）存储的日期
 * @param	无
 * @retval 	无
 * @note 	设置结束后，将单片机数据区（RAM）存储的日期存储到DS1302
 */
void TimeSet(void) {
	if (KeyNum == 2) { // 选择设置位
		++TimeSetSelect;
		TimeSetSelect %= 6;
	}
	if (KeyNum == 3) { // 设置对应数字增加
		++DS1302_Time[TimeSetSelect];
		if (DS1302_Time[0] > 99) {DS1302_Time[0] = 0;}
		if (DS1302_Time[1] > 12) {DS1302_Time[1] = 1;}
		if (DS1302_Time[2] > days_of_month(DS1302_Time[0], DS1302_Time[1])) {DS1302_Time[2] = 1;}
		if (DS1302_Time[3] > 23) {DS1302_Time[3] = 0;}
		if (DS1302_Time[4] > 59) {DS1302_Time[4] = 0;}
		if (DS1302_Time[5] > 59) {DS1302_Time[5] = 0;}
	}
	if (KeyNum == 4) { // 设置对应数字减小
		--DS1302_Time[TimeSetSelect];
		if (DS1302_Time[0] < 0) {DS1302_Time[0] = 99;}
		if (DS1302_Time[1] < 1) {DS1302_Time[1] = 12;}
		if (DS1302_Time[2] < 1) {DS1302_Time[2] = days_of_month(DS1302_Time[0], DS1302_Time[1]);}
		if (DS1302_Time[3] < 0) {DS1302_Time[3] = 23;}
		if (DS1302_Time[4] < 0) {DS1302_Time[4] = 59;}
		if (DS1302_Time[5] < 0) {DS1302_Time[5] = 59;}
	}
	// 实现设置模式选中闪烁
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
	//LCD_ShowNum(2, 10, TimeSetSelect, 2); // (debug)显示当前设置位
	//LCD_ShowNum(2, 13, TimeSetFlashFlag, 2); // (debug)显示当前闪烁标志 
}

void main() {
	LCD_Init();
	DS1302_Init();
	Timer0Init();
	LCD_ShowString(1, 1, "  -  -  ");
	LCD_ShowString(2, 1, "  :  :  ");	

//	DS1302_WriteByte(0x8E, 0x00); // 解除DS1302写保护
														  
	DS1302_SetTime(); // 使用单片机数据区（EEPROM）存储的时间数据设置DS1302初始值
	while (1) {
		KeyNum = Key();
		if (KeyNum == 1) { // 模式切换
			if (MODE == 0) { // 显示模式 -> 设置模式
				MODE = 1;
				TimeSetSelect = 0; // 进入设置模式时，设置位数清零
			} else if (MODE == 1) { // 设置模式 -> 显示模式 
				MODE = 0;
				DS1302_SetTime(); // 退出设置模式时，保存设置 
			}
		}
		switch (MODE) {
			case 0: TimeShow(); break; // 单片机工作在显示模式
			case 1: TimeSet(); break;  // 单片机工作在设置模式
		}
	}
}

/**
 * @brief	显示设置模式下，对应正在设置位闪烁
 			每1ms触发一次该定时器中断函数并计数1次
			计数达到500次（500ms）对应正在设置位闪烁标志位（TimeSetFlashFlag）翻转一次
 * @note  	该定时器中断函数不断被触发，即闪烁标志位（TimeSetFlashFlag）不断翻转
 			但只在设置模式下使用闪烁标志位（TimeSetFlashFlag），
			控制对应位闪烁闪烁标志位（TimeSetFlashFlag）
 */
void Timer0_Routine() interrupt 1 {
	static unsigned int T0Count;
	// 设施T0Count每个1ms加1
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	++T0Count;
	if (T0Count >= 500) {
		T0Count = 0;
		TimeSetFlashFlag = !TimeSetFlashFlag;
	}	
}