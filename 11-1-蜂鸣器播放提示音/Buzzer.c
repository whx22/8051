#include <REGX52.H>
#include <INTRINS.H>

/**
 * @brief	单片机通过P2^5端口控制ULN2003D驱动芯片驱动控制蜂鸣器发声
 */
sbit Buzzer = P2^5;

/**
 * @brief	蜂鸣器私有延时函数，延时500us，频率2000Hz
 * @param	无
 * @retval 	无
 */
void Buzzer_Delay500us()		//@12.000MHz
{
	unsigned char i;

	_nop_(); // 延时一个机器周期，head file: INTRINS.H
	i = 247;
	while (--i);
}

/**
 * @brief	蜂鸣器鸣响对应时间
 * @param	蜂鸣器鸣响时间（单位：ms）
 * @retval 	无
 */
void Buzzer_Time(unsigned int ms) {
	unsigned int i;
	for (i = 0; i < ms * 2; ++i) { // 翻转ms * 2次，蜂鸣器工作ms(ms)
		Buzzer = !Buzzer;
		Buzzer_Delay500us(); // 每隔500us翻转1次，周期1ms，频率1000Hz
	}
}