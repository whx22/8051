#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "Nixie.h"
#include "Key.h"

sbit DA = P2^1;
sbit LED = P2^0;

unsigned char Counter, Compare, Speed;
unsigned char i;

void main() {
  Timer0_Init();
  while (1) {
    for (i = 0; i < 100; ++i) {
      Compare = i;
      Delay(10);
    }
    for (i = 100; i > 0; --i) {
      Compare = i;
      Delay(10);
    }
  }
}

/**
 * @brief ÿ��100us����һ���жϴ����� 
 *  
 */
void Timer0_Routine() interrupt 1 {
	TL0 = 0x9C;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
  ++Counter;
  Counter %= 100;
  if (Counter < Compare) {
    DA = 1; // �������
  } else {
    DA = 0; // ���������
  }
}