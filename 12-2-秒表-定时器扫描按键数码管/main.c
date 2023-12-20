#include <REGX52.H>
#include "Timer0.h"
#include "Key.h"
#include "Nixie.h"
#include "Delay.h"
#include "AT24C02.h"

unsigned char KeyNum; // 被按下的按键键值
unsigned char Min, Sec, MiniSec; // 秒表时间数据
unsigned RunFlag; // 秒表运行标志

void main() {
	Timer0_Init();
	while (1) {
		KeyNum = Key();
		if (KeyNum == 1) { // 秒表工作状态转换
			RunFlag = !RunFlag;
		}
		if (KeyNum == 2) { // 秒表清零
			Min = 0;
			Sec = 0;
			MiniSec = 0;
		}
		if (KeyNum == 3) { // 秒表计时写入AT24C02
			AT24C02_WriteByte(0, Min);
			Delay(5);
			AT24C02_WriteByte(1, Sec);
			Delay(5);
			AT24C02_WriteByte(2, MiniSec);
			Delay(5);
		}
		if (KeyNum == 4) { // 从AT24C02读出数据作为秒表计时
		  	Min = AT24C02_ReadByte(0);
			Sec = AT24C02_ReadByte(1);
			MiniSec = AT24C02_ReadByte(2);
		}
		// 数码管显示设置
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
 * @brief 	秒表计时
 * @param	void
 * @retval  void
 * @caller	Timer0_Routine() interrupt 1
 * @callee  null
 * @note 	Timer0_Routine中每间隔10ms触发一次该函数，
 			仅工作在秒表运行模式
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
 * @brief 	定时器0中断处理函数	
 * @param	void
 * @retval	void
 * @caller 	hardware
 * @callee 	null
 */
void Timer0_Routine(void) interrupt 1 {
	static unsigned int T0Count1, T0Count2, T0Count3;
	// 设施T0Count每个1ms加1
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	
	// 中断实现按键扫描
	++T0Count1;
	if (T0Count1 >= 20) {
		T0Count1 = 0;
		Key_Loop();
	}

	// 中断实现数码管扫描
	++T0Count2;
	if (T0Count2 >= 2) {
		T0Count2 = 0;
		Nixie_Loop();
	}

	// 中断实现时间计数
	++T0Count3;
	if (T0Count3 >= 10) {
		T0Count3 = 0;
		Second_Loop();
	}
}