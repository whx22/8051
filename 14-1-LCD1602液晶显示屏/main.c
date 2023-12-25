#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"

void main() {
  LCD_Init();
  LCD_ShowChar(1, 1, 'A');
  LCD_ShowString(1, 3, "Hello");
  LCD_ShowNum(1, 9, 66, 2);
  LCD_ShowSignedNum(1, 12, -88, 2);
  LCD_ShowHexNum(2, 1, 0xA5, 2);
  LCD_ShowBinNum(2, 4, 0xA5, 8);
  LCD_ShowChar(2, 13, 0xDF); // 显示上标 `。`
  LCD_ShowChar(2, 14, 'C');
  LCD_ShowString(1, 16, "Welcome to China!");
  while (1) {
    LCD_WriteCommand(0x18); // 画面向左平移一个字符，但光标不动
    Delay(500);
  }
} 