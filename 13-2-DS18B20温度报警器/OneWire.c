#include <REGX52.H>

sbit OneWire_DQ = P3^7;

/**
 * @brief   ��ֹ�жϴ������ƻ������ߴ���ʱ��
 *          �ڵ��������ݴ���ʱ�ر��ж�
 */


/**
 * @brief   �����߳�ʼ��ʱ��
 * 
 * @return  �ӻ�����������Ӧ�ź�
 */
unsigned char OneWire_Init(void) {
    unsigned char i, AckBit;
    EA = 0; // �ر������ж�
    // �����ͷ����߿���Ȩ����ʼ������
    OneWire_DQ = 1;
    // ������ʼ��ӻ��������ݴ��俪ʼ��ʼ���ź�
    OneWire_DQ = 0;
    i = 247;while (--i);		//Delay 500us
    // �����ͷ����߿���Ȩ���ȴ��ӻ���Ӧ�ź�
    OneWire_DQ = 1;
    i = 32;while (--i);			//Delay 70us
    // �������ܴӻ���Ӧ�ź�
    AckBit = OneWire_DQ;
    i = 247;while (--i);		//Delay 500us
    EA = 1; // ���������ж�
    return AckBit;
}

/**
 * @brief   ������ӻ�����1bit����ʱ������60us��
 * 
 * @param   �������͵��������ϵ�1bit���ݣ��ӻ����ܣ�
 */
void OneWire_SendBit(unsigned char Bit) {
    unsigned char i;
    EA = 0; // �ر������ж�
    // �����������߿���Ȩ����ʾ�ӻ����ݴ��俪ʼ
    OneWire_DQ = 0;
	i = 4;while (--i);			//Delay 10us
    // ���������߷���1bit����
    OneWire_DQ = Bit;
	i = 24;while (--i);			//Delay 50us
    // �����ͷ����߿���Ȩ
    OneWire_DQ = 1;
    EA = 1; // ���������ж�
}

/**
 * @brief   ������ȡ�ӻ����͵�1bit����ʱ������60us��
 * 
 * @return  �����ӵ������϶�ȡ��1bit���ݣ��ӻ����ͣ�
 */
unsigned char OneWire_ReceiveBit(void) {
    unsigned char i, Bit;
    EA = 0; // �ر������ж�
    // �����������߿���Ȩ����ʾ�ӻ����ݴ��俪ʼ
    OneWire_DQ = 0;
    i = 2;while (--i);			//Delay 5us
    // �����ͷ����ߣ��ȴ��ӻ���������
	i = 2;while (--i);			//Delay 5us
    OneWire_DQ = 1;
    // ���������߽��в��������ܴӻ�����1bit����
    Bit = OneWire_DQ;
	i = 24;while (--i);			//Delay 50us
    EA = 1; // ���������ж�
    return Bit;
}

/**
 * @brief   ������ӻ�����1Byte����ʱ�򣨵�λ��ǰ��
 * 
 * @param   �������͵��������ϵ�1Byte���ݣ��ӻ����ܣ�
 */
void OneWire_SendByte(unsigned char Byte) {
    unsigned char i;
    for (i = 0; i < 8; ++i) {
        OneWire_SendBit(Byte & (0x01 << i));
    }
}

/**
 * @brief   ������ȡ�ӻ����͵�1Byte����ʱ�򣨵�λ��ǰ��
 * 
 * @return  �����ӵ������϶�ȡ��1Byte���ݣ��ӻ����ͣ�
 */
unsigned char OneWire_ReceiveByte(void) {
    unsigned char i, Byte = 0x00;
    for (i = 0; i < 8; ++i) {
        if (OneWire_ReceiveBit()) {
            Byte |= (0x01 << i);
        }
    }
    return Byte;
}