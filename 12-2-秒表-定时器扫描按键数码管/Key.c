#include <REGX52.h>
#include "Delay.h"

unsigned char Key_KeyNumber;

/**
 * @brief 	返回按下的键码值，并重置全局变量Key_KeyNumber
 * @param	void
 * @retval 	按下的按键键码
 * @caller	main
 * @callee	null
 */
unsigned char Key(void) {
	// 实现按键只触发一次，后置0，使用Temp保存，并重置全局变量Key_KeyNumber
	unsigned char Temp = 0;
	Temp = Key_KeyNumber;
	Key_KeyNumber = 0;
	return Temp;
}

/**
 * @brief 	获取按键状态函数
 * @param	void
 * @retval	按键状态
 * @caller	Key_Loop
 * @callee	null
 * @note	按键按下：按键对应端口为低电平
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
 * @brief 	按键扫描，将全局变量Key_GetState设置为获取到的键码
 * @param	void
 * @retval	void
 * @caller	Timer0_Routine interrupt 1
 * @callee	Key_GetState
 * @note 	通过判断按键状态是否存在从按下到松手的状态转换，实现松手时相应
 			Timer0_Routine中每间隔20ms触发一次该函数，
			实现不断扫描按键而不阻塞，按键按下和松开的防抖
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