#include <REGX52.H>

void Int0_Init(void) {
  // 配置外部中断
  IT0 = 1;  // 外部中断触发方式：1下降沿触发，0低电平触发 
  IE0 = 0;  // 外部中断标志清零
  EX0 = 1;  // 开启外部中断0
  EA = 1;   // 开启所有中断
  PX0 = 1;  // 高中断优先级
}

/*
void Int0_Rountine(void) interrupt 0 {
  
}
*/
