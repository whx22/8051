#include <REGX52.H>
#include "Delay.h"

sbit RCK = P3^5; // RCLK 	register clock 数据写入边沿触发信号端口（一次写入八位）
sbit SCK = P3^6; // SRCLK 	数据移位边沿触发信号端口
sbit SER = P3^4; // SER		数据信号端口
#define MATRIX_LED_PORT P0

/**
 * @brief  74HC595写入一个字节
 * @param  需要写入的字节
 * @retval 无
 */
void _74HC595_WriteByte(unsigned char Byte) {
	unsigned char i;
	for (i = 0; i < 8; ++i) {
		SER = Byte & (0x80 >> i);
		SCK = 1;
		SCK = 0;
	}
	RCK = 1;
	RCK = 0;
}

/**
 * @brief  LED点阵屏初始化
 * @param  无
 * @retval 无
 */
void MatrixLED_Init() {
	SCK = 0; // 初始化上升沿移位
	RCK = 0; // 初始化上升沿锁存
}

/**
 * @brief  显示一列数据
 * @param  需要显示数据的列（范围0 - 7）
 * @param  需要显示的数据 （高位在上，1为亮，0为灭）
 * @retval 无
 */
void MatrixLED_ShowColumn(unsigned char Column,Data) {
	_74HC595_WriteByte(Data);
	MATRIX_LED_PORT = ~(0x80 >> Column); // 列选
	Delay(1); // 延时显示
	MATRIX_LED_PORT = 0xFF; // 列选清零
}