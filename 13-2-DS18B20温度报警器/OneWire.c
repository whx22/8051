#include <REGX52.H>

sbit OneWire_DQ = P3^7;

/**
 * @brief   防止中断处理函数破坏单总线传输时序
 *          在单总线数据传输时关闭中断
 */


/**
 * @brief   单总线初始化时序
 * 
 * @return  从机对主机的响应信号
 */
unsigned char OneWire_Init(void) {
    unsigned char i, AckBit;
    EA = 0; // 关闭所有中断
    // 主机释放总线控制权，初始化总线
    OneWire_DQ = 1;
    // 主机开始向从机发送数据传输开始初始化信号
    OneWire_DQ = 0;
    i = 247;while (--i);		//Delay 500us
    // 主机释放总线控制权，等待从机响应信号
    OneWire_DQ = 1;
    i = 32;while (--i);			//Delay 70us
    // 主机接受从机响应信号
    AckBit = OneWire_DQ;
    i = 247;while (--i);		//Delay 500us
    EA = 1; // 开启所有中断
    return AckBit;
}

/**
 * @brief   主机向从机发送1bit数据时序（至少60us）
 * 
 * @param   主机发送到单总线上的1bit数据（从机接受）
 */
void OneWire_SendBit(unsigned char Bit) {
    unsigned char i;
    EA = 0; // 关闭所有中断
    // 主机请求总线控制权，提示从机数据传输开始
    OneWire_DQ = 0;
	i = 4;while (--i);			//Delay 10us
    // 主机向总线发送1bit数据
    OneWire_DQ = Bit;
	i = 24;while (--i);			//Delay 50us
    // 主机释放总线控制权
    OneWire_DQ = 1;
    EA = 1; // 开启所有中断
}

/**
 * @brief   主机读取从机发送的1bit数据时序（至少60us）
 * 
 * @return  主机从单总线上读取的1bit数据（从机发送）
 */
unsigned char OneWire_ReceiveBit(void) {
    unsigned char i, Bit;
    EA = 0; // 关闭所有中断
    // 主机请求总线控制权，提示从机数据传输开始
    OneWire_DQ = 0;
    i = 2;while (--i);			//Delay 5us
    // 主机释放总线，等待从机发送数据
	i = 2;while (--i);			//Delay 5us
    OneWire_DQ = 1;
    // 主机对总线进行采样，接受从机发送1bit数据
    Bit = OneWire_DQ;
	i = 24;while (--i);			//Delay 50us
    EA = 1; // 开启所有中断
    return Bit;
}

/**
 * @brief   主机向从机发送1Byte数据时序（低位在前）
 * 
 * @param   主机发送到单总线上的1Byte数据（从机接受）
 */
void OneWire_SendByte(unsigned char Byte) {
    unsigned char i;
    for (i = 0; i < 8; ++i) {
        OneWire_SendBit(Byte & (0x01 << i));
    }
}

/**
 * @brief   主机读取从机发送的1Byte数据时序（低位在前）
 * 
 * @return  主机从单总线上读取的1Byte数据（从机发送）
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