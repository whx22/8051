#include <REGX52.H>
#include "I2C.h"

#define AT24C02_ADDRESS 0xA0

/**
 * @brief 	单片机向AT24C02写入1Byte的数据
 * @param1	写入到AT24C02的内部地址
 * @param2  写入的数据
 * @retval	无
 */
void AT24C02_WriteByte(unsigned char WordAddress, Data) {
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck(); // 需要考虑处理从机发送非应答的情况：重新发送数据或其他
   	I2C_SendByte(WordAddress);
	I2C_ReceiveAck();
	I2C_SendByte(Data);
	I2C_ReceiveAck();
	I2C_Stop();
}

/**
 * @brief 	单片机向AT24C02读取1Byte的数据
 * @param1	读取的AT24C02的内部地址
 * @retval	返回读取到的数据
 */
unsigned char AT24C02_ReadByte(unsigned char WordAddress) {
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck(); // 需要考虑处理从机发送非应答的情况：重新发送数据或其他
   	I2C_SendByte(WordAddress);
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS | 0x01); // 写地址转换为读地址
	I2C_ReceiveAck();
	Data = I2C_ReceiveByte();
	I2C_SendAck(1); // 最后一次读，发送非应答
	I2C_Stop();
	return Data;
}