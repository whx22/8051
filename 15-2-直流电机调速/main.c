#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "Nixie.h"
#include "Key.h"

sbit Motor = P1^0;
sbit LED = P2^0;

unsigned char Counter, Compare, KeyNum, Speed;

void main() {
  Timer0_Init();
  while (1) {
    KeyNum = Key();
    if (KeyNum == 1) {
      ++Speed;
      Speed %= 4;
      if (Speed == 0) { Compare = 0; }
      if (Speed == 1) { Compare = 50; }
      if (Speed == 2) { Compare = 75; }
      if (Speed == 3) { Compare = 100; }
    }
    Nixie(1, Speed);
  }
}

/**
 * @brief 每隔100us进入一次中断处理函数 
 *  
 */
void Timer0_Routine() interrupt 1 {
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
  ++Counter;
  Counter %= 100;
  if (Counter < Compare) {
    Motor = 1; // 电机工作
  } else {
    Motor = 0; // 电机不工作
  }
}