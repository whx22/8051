#include <REGX52.H>

/**
 * @brief  定时器1初始化 100us@12.000MHz
 * @param  void
 * @retval void
 */
void Timer1_Init(void)		//100us@12.000MHz
{
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0x9C;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1; 	// 开启timer_1中断
	EA = 1; 	// 开启所有中断
	PT1 = 0; 	// 设置timer_1优先级为0
}

/* 
// timer1 中断服务程序模板（中断号：3）
void Timer0_Routine() interrupt 3 {
	static unsigned int T1Count;
	// 设施T0Count每个1ms加1
	TL1 = 0x9C;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	++T1Count;
	if (T1Count >= 1000) {
		T1Count = 0;
	}	
}
*/