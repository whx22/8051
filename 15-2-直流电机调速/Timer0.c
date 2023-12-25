#include <REGX52.H>

/**
 * @brief  定时器0初始化 100us@12.000MHz
 * @param  void
 * @retval void
 */
void Timer0_Init(void)		//100us@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1; 	// 开启timer_0中断
	EA = 1; 	// 开启所有中断
	PT0 = 0; 	// 设置timer_0优先级为0
}

/* 
// timer0 中断服务程序模板（中断号：1）
void Timer0_Routine() interrupt 1 {
	static unsigned int T0Count;
	// 设施T0Count每个1ms加1
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	++T0Count;
	if (T0Count >= 1000) {
		T0Count = 0;
	}	
}
*/