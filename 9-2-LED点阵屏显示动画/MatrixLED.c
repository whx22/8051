#include <REGX52.H>
#include "Delay.h"

sbit RCK = P3^5; // RCLK 	register clock ����д����ش����źŶ˿ڣ�һ��д���λ��
sbit SCK = P3^6; // SRCLK 	������λ���ش����źŶ˿�
sbit SER = P3^4; // SER		�����źŶ˿�
#define MATRIX_LED_PORT P0

/**
 * @brief  74HC595д��һ���ֽ�
 * @param  ��Ҫд����ֽ�
 * @retval ��
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
 * @brief  LED��������ʼ��
 * @param  ��
 * @retval ��
 */
void MatrixLED_Init() {
	SCK = 0; // ��ʼ����������λ
	RCK = 0; // ��ʼ������������
}

/**
 * @brief  ��ʾһ������
 * @param  ��Ҫ��ʾ���ݵ��У���Χ0 - 7��
 * @param  ��Ҫ��ʾ������ ����λ���ϣ�1Ϊ����0Ϊ��
 * @retval ��
 */
void MatrixLED_ShowColumn(unsigned char Column,Data) {
	_74HC595_WriteByte(Data);
	MATRIX_LED_PORT = ~(0x80 >> Column); // ��ѡ
	Delay(1); // ��ʱ��ʾ
	MATRIX_LED_PORT = 0xFF; // ��ѡ����
}