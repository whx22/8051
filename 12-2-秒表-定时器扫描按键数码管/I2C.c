#include <REGX52.H>

sbit I2C_SCL = P2^1;
sbit I2C_SDA = P2^0;

/**
 * @brief 	主机发送数据传输开始信号
 * @param	无
 * @retval	无
 */
void I2C_Start(void) {
	// 保证开始信号之前，SDA、SCL处于高电平
	I2C_SDA = 1;
	I2C_SCL = 1;
	// 开始信号：SCL高电平时SDA产生一个下降沿信号
	// SDA先拉低，SCL后拉低
	I2C_SDA = 0;
	I2C_SCL = 0;
}

/**
 * @brief 	主机发送数据传输结束信号
 * @param	无
 * @retval	无
 */
void I2C_Stop(void) {
	// 保证停止之前，SDA处于低电平
	I2C_SDA = 0;
	// 停止信号：SCL高电平时SDA产生一个上升沿信号
	// SCL先拉高，SDA后拉高
	I2C_SCL = 1;
	I2C_SDA = 1;
}

/**
 * @brief 	主机向从机发送1Byte的数据
 * @param   需要发送的数据
 * @retval	无
 */
void I2C_SendByte(unsigned char Byte) {
	unsigned char i;
	for (i = 0; i < 8; ++i) {
		// 从高位到低位，依次发送
		I2C_SDA = Byte & (0x80 >> i);
		// 根据AT24C02交流电气特性，
		// STC89C52单片机执行语句周期小于AT24C02读取周期
		// 程序中直接翻转SCL，不适用Delay函数，
		// AT24C02可以直接实现读取数据
		// note1: 高速单片机，需要查阅手册，实现传输和接受数据速率匹配
		// note2: AT24C02写周期，接受周期需要5ms，需要对应的Delay函数
		I2C_SCL = 1;
		I2C_SCL = 0;	
	}
}

/**
 * @brief 	主机接受从机发送的1Byte数据
 * @param	接受到的1Byte数据
 * @retval	无
 */
unsigned char I2C_ReceiveByte(void) {
	unsigned char i = 0, Byte = 0x00;

  	// 主机释放SDA总线控制权，交给从机发送数据
	I2C_SDA = 1;

	for (i = 0; i < 8; ++i) {
		// 主机接受从机数据，从高位到低位，依次发送
		I2C_SCL = 1;
		if (I2C_SDA) { Byte |= (0x80 >> i);}
		I2C_SCL = 0;
	}
	return Byte;
}

/**
 * @brief 	主机接受从机发送的1Byte数据后，主机向从机发送数据接受完成的应答信号
 * @param	主机应答信号的具体值，应答：0，不应答：1
 * @retval	无
 */
void I2C_SendAck(unsigned char AckBit) {
	I2C_SDA = AckBit; // 应答：0，不应答：1
	I2C_SCL = 1;
	I2C_SCL = 0;
}

/**
 * @brief 	主机向从机发送1Byte的数据，从机向主机发送数据接受完成的应答信号
 * @param	无
 * @retval	从机发送的应答信号的具体值，应答：0，不应答：1
 */
unsigned char I2C_ReceiveAck(void) {
	unsigned char AckBit = 0;	
	// 主机释放SDA总线控制权，交给从机发送应答信号
	I2C_SDA = 1;
	// 主机接受从机应答并返回
	I2C_SCL = 1;
	AckBit = I2C_SDA;
	I2C_SCL = 0;
	return AckBit;
}