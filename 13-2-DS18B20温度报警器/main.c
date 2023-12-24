#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "Key.h"
#include "AT24C02.h"
#include "DS18B20.h"
#include "Timer0.h"

float T, TShow;
char TLOW, THIGH;
unsigned char KeyNum;

void main() {
  DS18B20_ConvertT();
  Delay(1000);
  TLOW = AT24C02_ReadByte(0);
  THIGH = AT24C02_ReadByte(1);
  if (TLOW < -55 || THIGH > 125 || TLOW > THIGH) {
    TLOW = 20;
    THIGH = 28;
  }
  LCD_Init();
  LCD_ShowString(1, 1, "T:");
  LCD_ShowString(2, 1, "TL:");
  LCD_ShowString(2, 9, "TH:");
  LCD_ShowSignedNum(2, 4, TLOW, 3);
  LCD_ShowSignedNum(2, 12, THIGH, 3);
  Timer0_Init();
  while (1) {
    KeyNum = Key();

    // �¶ȶ�ȡ����ʾ
    DS18B20_ConvertT();
    T = DS18B20_ReadT();
    if (T < 0) {
      LCD_ShowChar(1, 3, '-');
      TShow = -T;
    } else {
      LCD_ShowChar(1, 3, '+');
      TShow = T;
    }
    LCD_ShowNum(1, 4, TShow, 3);
    LCD_ShowChar(1, 7, '.');
    LCD_ShowNum(1, 8, (unsigned long)(TShow * 100) % 100, 2);

    // ��ֵ�жϼ���ʾ
    if (KeyNum) {
      if (KeyNum == 1) {
        ++TLOW;
        if (TLOW > THIGH) { --TLOW; }
      }
      if (KeyNum == 2) {
        --TLOW;
        if (TLOW < -55) { TLOW = -55; }      
      }
      if (KeyNum == 3) {
        ++THIGH;
        if (THIGH > 125) { THIGH = 125; }            
      }
      if (KeyNum == 4) {
        --THIGH;
        if (THIGH < TLOW) { ++THIGH; }
      }
    }
    LCD_ShowSignedNum(2, 4, TLOW, 3);
    LCD_ShowSignedNum(2, 12, THIGH, 3);
	  // ���¶���ֵ�洢��AT24C02��
    AT24C02_WriteByte(0, TLOW);
    Delay(5);
    AT24C02_WriteByte(1, THIGH);
    Delay(5);
    if (T < TLOW) {
      LCD_ShowString(1, 13, "OV:L");
    } else if (T > THIGH) {
      LCD_ShowString(1, 13, "OV:H");
    } else {
      LCD_ShowString(1, 13, "    ");
    }
  }
}

// timer0 �жϷ������ģ�壨�жϺţ�1��
void Timer0_Routine() interrupt 1 {
	static unsigned int T0Count;
	// ��ʩT0Countÿ��1ms��1
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	++T0Count;
	if (T0Count >=  20) {
		T0Count = 0;
    Key_Loop();
	}	
}