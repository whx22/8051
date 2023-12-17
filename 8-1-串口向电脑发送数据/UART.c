#include <REGX52.H>

/**
 * @brief  串口初始化 4800bps@12.000MHz
 * @param  无
 * @retval 无
 */
void UART_Init() {
// 串口通信控制寄存器SCON
	// 工作模式1：SM0 = 0 SM1 = 1
	// 接受不使能 REN = 0
	SCON = 0x40;
// 电源控制寄存器PCON
	// 波特率加倍SMOD = 1
	PCON |= 0x80;		//使能波特率倍速位SMOD = 1	
// 配置定时器
	TMOD &= 0x0F;		//设置timer1模式
	TMOD |= 0x20;		//设置timer1模式为8位自动重装
	TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}

/**
 * @brief  串口向上位机发送一个字节的数据
 * @param  需要发送的一个字节数据
 * @retval 无
 */
void UART_SendByte(unsigned char Byte) {
	SBUF = Byte;
	while(TI == 0);
	TI = 0;
}