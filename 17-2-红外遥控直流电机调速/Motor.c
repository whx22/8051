#include <REGX52.H>
#include "Timer1.h"

sbit Motor = P1^0;

unsigned char Counter, Compare;

/**
 * @brief 直流电机初始化
 * 
 */
void Motor_Init(void) {
  Timer1_Init();
}

/**
 * @brief 设置直流电机速度
 * 
 * @param Speed 在main.c中定义，需要设置的速度
 */
void Motor_SetSpeed(unsigned char Speed) {
	Compare = Speed;
}  

/**
 * @brief 每隔100us进入一次中断处理函数 
 *  
 */
void Timer1_Routine() interrupt 3 {
	TL1 = 0x9C;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
  ++Counter;
  Counter %= 100;
  if (Counter < Compare) {
    Motor = 1; // 电机工作
  } else {
    Motor = 0; // 电机不工作
  }
}