#include <REGX52.H>
#include "OneWire.h"
#include "LCD1602.h"

// DS18B20 ROM指令
//  跳过ROM地址识别，单总线上只挂载一个DS18B20芯片时使用
#define DS18B20_SKIP_ROM			    0xCC

// DS18B20 功能指令
//  DS18B20将温度数据模拟量转化为数字量并存储到RAM中
#define DS18B20_CONVERT_T			    0x44
//  主机读取DS18B20的RAM数据
#define DS18B20_READ_SCRATCHPAD   0xBE

/**
 * @brief 主机向从机发送温度转化指令
 *        DS18B20将温度数据模拟量转化为温度数据数字量
 *        并存储到RAM中
 */
void DS18B20_ConvertT(void) {
  OneWire_Init();
  OneWire_SendByte(DS18B20_SKIP_ROM);
  OneWire_SendByte(DS18B20_CONVERT_T);
}

/**
 * @brief   主机读取从机RAM，仅接受前2Byte
 *          温度数据数字量（最高5位：符号位，中间7位：整数部分，最低4位：小数部分）
 * @return  
 */
float DS18B20_ReadT(void) {
  unsigned char TLSB, TMSB;
  int Temp;
  float T;
  OneWire_Init();
  OneWire_SendByte(DS18B20_SKIP_ROM);
  OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
  TLSB = OneWire_ReceiveByte();
  TMSB = OneWire_ReceiveByte();
  // // debug 显示接受到的数据帧
  // LCD_ShowBinNum(1, 1, TMSB, 8); // debug
  // LCD_ShowBinNum(1, 9, TLSB, 8); // debug
  Temp = (TMSB << 8) | TLSB;
  T = Temp / 16.0;
  return T;
}