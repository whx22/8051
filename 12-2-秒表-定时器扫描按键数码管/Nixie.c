#include <REGX52.H>
#include "Delay.h"

// 数码管显示缓存数组
unsigned char Nixie_Buf[9] = {0, 10, 10, 10, 10, 10, 10, 10, 10};

// 数码管对应数字对应的段码表
unsigned char NixieTable[] = \
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x40};
// 0     1     2     3     4     5     6     7     8     9	  空  	 -


/**
 * @brief 	将指定位设置为需要显示的数据
 * @param	Location: 指定显示的数码管
 * @param	Number: 指定数码管显示的数字
 * @retval	void
 * @caller	main
 * @callee 	null
 */
void Nixie_SetBuf(unsigned char Location, Number) {
	Nixie_Buf[Location] = Number;	
}

/**
 * @brief 	指定的数码管上显示指定数据
 * @param	Location: 指定显示的数码管
 * @param	Number: 指定数码管显示的数字
 * @retval	void
 * @caller  Nixie_Loop
 * @callee	null
 */
void Nixie_Scan(unsigned char Location, Number) {
	P0 = 0x00; // 断码清零
	// 根据具体Location参数，选中对应数码管
	switch(Location) {
		case 1: P2_4 = 1, P2_3 = 1, P2_2 = 1; break; // 第一个数码管：LED8，Y7 
		case 2: P2_4 = 1, P2_3 = 1, P2_2 = 0; break; 
		case 3: P2_4 = 1, P2_3 = 0, P2_2 = 1; break;
		case 4: P2_4 = 1, P2_3 = 0, P2_2 = 0; break;
		case 5: P2_4 = 0, P2_3 = 1, P2_2 = 1; break;
		case 6: P2_4 = 0, P2_3 = 1, P2_2 = 0; break;
		case 7: P2_4 = 0, P2_3 = 0, P2_2 = 1; break;
		case 8: P2_4 = 0, P2_3 = 0, P2_2 = 0; break;
	}
	// 根据Number参数显示对应数字
	P0 = NixieTable[Number];
}

/**
 * @brief 	数码管的每一位显示对应位的数据
 * @param	void
 * @retval	void
 * @caller 	Timer0_Routine interrupt 1
 * @callee	Nixie_Scan
 * @note	Timer0_Routine中每间隔2ms触发一次该函数，
 			实现数码管动态刷新显示，不被其他高cpu占用指令影响正常显示
 */
void Nixie_Loop(void) {
	static unsigned char i = 1 ;
	Nixie_Scan(i, Nixie_Buf[i]);
	++i;
	if (i > 8) { // 逐位显示缓存数组中的内容
		i = 1;
	}
}