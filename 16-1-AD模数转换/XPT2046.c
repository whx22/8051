#include <REGX52.H>

sbit XPT2046_CS = P3^5;
sbit XPT2046_DCLK = P3^6;
sbit XPT2046_DIN = P3^4;
sbit XPT2046_DOUT = P3^7;

/**
 * @brief 读取XPT2046模拟量转化为数字量的结果
 * 
 * @param Command 控制字：选择写入模拟量通道 
 * @return unsigned int 模拟量转化结果（数字量）
 * @note SPI总线传输协议，高位在前
 */
unsigned int XPT2046_ReadAD(unsigned char Command) {
  unsigned int ADValue = 0;
  unsigned char i;
  // 开始数据传输
  XPT2046_CS = 0;
  XPT2046_DCLK = 0;

  // 向XPT2046写控制字（8位）
  for (i = 0; i < 8; ++i) {
    XPT2046_DIN = Command & (0x80 >> i);
    XPT2046_DCLK = 1;
    XPT2046_DCLK = 0;
  }
  // XPT2046 BUSY STC89C52可省略
  // 读取XPT2046转化结果（16位数字量）
  for (i = 0; i < 16; ++i) {
    XPT2046_DCLK = 1;
    XPT2046_DCLK = 0;
    if (XPT2046_DOUT) {
      ADValue |= (0x8000 >> i); 
    }
  }

  // 结束数据传输
  XPT2046_CS = 1;

  if (Command & 0x08) {
    return (ADValue >> 8);
  } else {
    return (ADValue >> 4);
  }
}