#include <REGX52.H>
#include "OneWire.h"
#include "LCD1602.h"

// DS18B20 ROMָ��
//  ����ROM��ַʶ�𣬵�������ֻ����һ��DS18B20оƬʱʹ��
#define DS18B20_SKIP_ROM			0xCC
// DS18B20 ����ָ��
//  DS18B20���¶�����ģ����ת��Ϊ���������洢��RAM��
#define DS18B20_CONVERT_T			0x44
//  ������ȡDS18B20��RAM����
#define DS18B20_READ_SCRATCHPAD 	0xBE

void DS18B20_ConvertT(void) {
  OneWire_Init();
  OneWire_SendByte(DS18B20_SKIP_ROM);
  OneWire_SendByte(DS18B20_CONVERT_T);
}

float DS18B20_ReadT(void) {
  unsigned char TLSB, TMSB;
  int Temp;
  float T;
  OneWire_Init();
  OneWire_SendByte(DS18B20_SKIP_ROM);
  OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
  TLSB = OneWire_ReceiveByte();
  TMSB = OneWire_ReceiveByte();
  LCD_ShowBinNum(1, 1, TMSB, 8);
  LCD_ShowBinNum(1, 9, TLSB, 8);
  Temp = (TMSB << 8) | TLSB;
  T = Temp / 16.0;
  return T;
}