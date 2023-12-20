#include <REGX52.H>
#include "I2C.h"

#define AT24C02_ADDRESS 0xA0

/**
 * @brief 	��Ƭ����AT24C02д��1Byte������
 * @param1	д�뵽AT24C02���ڲ���ַ
 * @param2  д�������
 * @retval	��
 */
void AT24C02_WriteByte(unsigned char WordAddress, Data) {
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck(); // ��Ҫ���Ǵ���ӻ����ͷ�Ӧ�����������·������ݻ�����
   	I2C_SendByte(WordAddress);
	I2C_ReceiveAck();
	I2C_SendByte(Data);
	I2C_ReceiveAck();
	I2C_Stop();
}

/**
 * @brief 	��Ƭ����AT24C02��ȡ1Byte������
 * @param1	��ȡ��AT24C02���ڲ���ַ
 * @retval	���ض�ȡ��������
 */
unsigned char AT24C02_ReadByte(unsigned char WordAddress) {
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck(); // ��Ҫ���Ǵ���ӻ����ͷ�Ӧ�����������·������ݻ�����
   	I2C_SendByte(WordAddress);
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS | 0x01); // д��ַת��Ϊ����ַ
	I2C_ReceiveAck();
	Data = I2C_ReceiveByte();
	I2C_SendAck(1); // ���һ�ζ������ͷ�Ӧ��
	I2C_Stop();
	return Data;
}