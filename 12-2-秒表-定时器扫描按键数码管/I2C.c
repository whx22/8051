#include <REGX52.H>

sbit I2C_SCL = P2^1;
sbit I2C_SDA = P2^0;

/**
 * @brief 	�����������ݴ��俪ʼ�ź�
 * @param	��
 * @retval	��
 */
void I2C_Start(void) {
	// ��֤��ʼ�ź�֮ǰ��SDA��SCL���ڸߵ�ƽ
	I2C_SDA = 1;
	I2C_SCL = 1;
	// ��ʼ�źţ�SCL�ߵ�ƽʱSDA����һ���½����ź�
	// SDA�����ͣ�SCL������
	I2C_SDA = 0;
	I2C_SCL = 0;
}

/**
 * @brief 	�����������ݴ�������ź�
 * @param	��
 * @retval	��
 */
void I2C_Stop(void) {
	// ��ֹ֤֮ͣǰ��SDA���ڵ͵�ƽ
	I2C_SDA = 0;
	// ֹͣ�źţ�SCL�ߵ�ƽʱSDA����һ���������ź�
	// SCL�����ߣ�SDA������
	I2C_SCL = 1;
	I2C_SDA = 1;
}

/**
 * @brief 	������ӻ�����1Byte������
 * @param   ��Ҫ���͵�����
 * @retval	��
 */
void I2C_SendByte(unsigned char Byte) {
	unsigned char i;
	for (i = 0; i < 8; ++i) {
		// �Ӹ�λ����λ�����η���
		I2C_SDA = Byte & (0x80 >> i);
		// ����AT24C02�����������ԣ�
		// STC89C52��Ƭ��ִ���������С��AT24C02��ȡ����
		// ������ֱ�ӷ�תSCL��������Delay������
		// AT24C02����ֱ��ʵ�ֶ�ȡ����
		// note1: ���ٵ�Ƭ������Ҫ�����ֲᣬʵ�ִ���ͽ�����������ƥ��
		// note2: AT24C02д���ڣ�����������Ҫ5ms����Ҫ��Ӧ��Delay����
		I2C_SCL = 1;
		I2C_SCL = 0;	
	}
}

/**
 * @brief 	�������ܴӻ����͵�1Byte����
 * @param	���ܵ���1Byte����
 * @retval	��
 */
unsigned char I2C_ReceiveByte(void) {
	unsigned char i = 0, Byte = 0x00;

  	// �����ͷ�SDA���߿���Ȩ�������ӻ���������
	I2C_SDA = 1;

	for (i = 0; i < 8; ++i) {
		// �������ܴӻ����ݣ��Ӹ�λ����λ�����η���
		I2C_SCL = 1;
		if (I2C_SDA) { Byte |= (0x80 >> i);}
		I2C_SCL = 0;
	}
	return Byte;
}

/**
 * @brief 	�������ܴӻ����͵�1Byte���ݺ�������ӻ��������ݽ�����ɵ�Ӧ���ź�
 * @param	����Ӧ���źŵľ���ֵ��Ӧ��0����Ӧ��1
 * @retval	��
 */
void I2C_SendAck(unsigned char AckBit) {
	I2C_SDA = AckBit; // Ӧ��0����Ӧ��1
	I2C_SCL = 1;
	I2C_SCL = 0;
}

/**
 * @brief 	������ӻ�����1Byte�����ݣ��ӻ��������������ݽ�����ɵ�Ӧ���ź�
 * @param	��
 * @retval	�ӻ����͵�Ӧ���źŵľ���ֵ��Ӧ��0����Ӧ��1
 */
unsigned char I2C_ReceiveAck(void) {
	unsigned char AckBit = 0;	
	// �����ͷ�SDA���߿���Ȩ�������ӻ�����Ӧ���ź�
	I2C_SDA = 1;
	// �������ܴӻ�Ӧ�𲢷���
	I2C_SCL = 1;
	AckBit = I2C_SDA;
	I2C_SCL = 0;
	return AckBit;
}