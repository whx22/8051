#include <REGX52.H>

sbit XPT2046_CS = P3^5;
sbit XPT2046_DCLK = P3^6;
sbit XPT2046_DIN = P3^4;
sbit XPT2046_DOUT = P3^7;

/**
 * @brief ��ȡXPT2046ģ����ת��Ϊ�������Ľ��
 * 
 * @param Command �����֣�ѡ��д��ģ����ͨ�� 
 * @return unsigned int ģ����ת���������������
 * @note SPI���ߴ���Э�飬��λ��ǰ
 */
unsigned int XPT2046_ReadAD(unsigned char Command) {
  unsigned int ADValue = 0;
  unsigned char i;
  // ��ʼ���ݴ���
  XPT2046_CS = 0;
  XPT2046_DCLK = 0;

  // ��XPT2046д�����֣�8λ��
  for (i = 0; i < 8; ++i) {
    XPT2046_DIN = Command & (0x80 >> i);
    XPT2046_DCLK = 1;
    XPT2046_DCLK = 0;
  }
  // XPT2046 BUSY STC89C52��ʡ��
  // ��ȡXPT2046ת�������16λ��������
  for (i = 0; i < 16; ++i) {
    XPT2046_DCLK = 1;
    XPT2046_DCLK = 0;
    if (XPT2046_DOUT) {
      ADValue |= (0x8000 >> i); 
    }
  }

  // �������ݴ���
  XPT2046_CS = 1;

  if (Command & 0x08) {
    return (ADValue >> 8);
  } else {
    return (ADValue >> 4);
  }
}