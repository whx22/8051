#include <REGX52.H>

sbit LED = P2^0;

void Delay(unsigned int t) {
  while (t--); // 此处t--和--t效果不同
}

void main() {
  unsigned char Time, i;
  while (1) {
    for (Time = 0; Time < 100; ++Time) { // 亮度变化(灭到亮)
      for (i = 0; i < 20; ++i) { // 延时
        LED = 0;  // LED亮
        Delay(Time);
        LED = 1;  // LED灭
        Delay(100 - Time);
      }
    }
    for (Time = 100; Time > 0; --Time) { // 亮度变化（亮到灭）
      for (i = 0; i < 20; ++i) { // 延时
        LED = 0; // LED亮
        Delay(Time);
        LED = 1; // LED灭
        Delay(100 - Time);
      }
    }
  }
}