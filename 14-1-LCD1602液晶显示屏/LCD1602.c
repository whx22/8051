#include <REGX52.H>

sbit LCD_WR = P2^5; 
sbit LCD_RS = P2^6;
sbit LCD_EN = P2^7;
#define LCD_DataPort P0

/**
 * @brief   LCD内部延时函数
 * 
 */
void LCD_Delay()		//@12.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}

/**
 * @brief   向LCD1602写命令（并行传输8bit命令字）
 * 
 * @param Command 向LCD1602传输的命令字
 */
void LCD_WriteCommand(unsigned char Command) {
  LCD_RS = 0;
  LCD_WR = 0;
  LCD_DataPort = Command;
  LCD_EN = 1;
  LCD_Delay();
  LCD_EN = 0;
  LCD_Delay();
}

/**
 * @brief   向LCD1602写数据（并行传输8bit数据）
 * 
 * @param Data 向LCD1602传输的数据
 */
void LCD_WriteData(unsigned char Data) {
  LCD_RS = 1;
  LCD_WR = 0;
  LCD_DataPort = Data;
  LCD_EN = 1;
  LCD_Delay();
  LCD_EN = 0;
  LCD_Delay();
}

/**
 * @brief LCD1602初始化
 * 
 */
void LCD_Init(void) {
  LCD_WriteCommand(0x38); // 八位数据接口，两行显示，5 * 7点阵
  LCD_WriteCommand(0x0C); // 显示开，光标关，闪烁关
  LCD_WriteCommand(0x06); // 数据读、写操作后，AC自动加一，画面不动
  LCD_WriteCommand(0x01); // 清屏：清除DDRAM和AC值，光标复位
}

/**
 * @brief LCD1602设置光标位置
 * 
 * @param Line    光标行位置
 * @param Column  光标列位置
 */
void LCD_SetCursor(unsigned char Line, Column) {
  if (Line == 1) {
    LCD_WriteCommand(0x80 | (Column - 1));
  } else {
    LCD_WriteCommand(0x80 | ((Column - 1) + 0x40));
  }
}

/**
 * @brief LCD1602显示一个字符
 * 
 * @param Line    字符行位置
 * @param Column  字符列位置
 * @param Char    字符
 */
void LCD_ShowChar(unsigned char Line, Column, char Char) {
  LCD_SetCursor(Line, Column);
  LCD_WriteData(Char);
}

/**
 * @brief LCD1602显示一个字符串
 * 
 * @param Line    字符串起始行位置
 * @param Column  字符串起始列位置
 * @param String  字符串
 */
void LCD_ShowString(unsigned char Line, Column, char *String) {
  unsigned char i = 0;
  LCD_SetCursor(Line, Column);
  while (String[i] != '\0') {
    LCD_WriteData(String[i]);
    ++i;
  }
}

/**
 * @brief 计算X^Y
 * 
 * @param X     底数
 * @param Y     指数
 * @return int  值
 */
int LCD_Pow(int X, int Y) {
  unsigned char i;
  int Result = 1;
  for (i = 0; i < Y; ++i) {
    Result *= X;
  }
  return Result;
}

/**
 * @brief LCD1602显示Length位无符号整数
 * 
 * @param Line    显示起始行位置
 * @param Column  显示起始列位置
 * @param Number  显示的数字
 * @param Length  显示数字的长度
 */
void LCD_ShowNum(unsigned char Line, Column, unsigned int Number, unsigned char Length) {
  unsigned char i;
  LCD_SetCursor(Line, Column);
  for (i = Length; i > 0; --i) {
    LCD_WriteData(0x30 + (Number / LCD_Pow(10, i - 1) % 10));
  }
}

/**
 * @brief LCD1602显示一个有符号整数
 * 
 * @param Line    显示起始行位置
 * @param Column  显示起始列位置
 * @param Number  显示的数字
 * @param Length  显示数字的长度（不包括符号位）
 */
void LCD_ShowSignedNum(unsigned char Line, Column, int Number, unsigned char Length) {
  unsigned char i = 0;
  unsigned int u_Number = 0;
  LCD_SetCursor(Line, Column);
  if (Number >= 0) {
    LCD_WriteData('+');
    u_Number = Number;
    
  } else {
    LCD_WriteData('-');
    u_Number = -Number;
  }
    for (i = Length; i > 0; --i) {
    LCD_WriteData(0x30 + (u_Number / LCD_Pow(10, i - 1) % 10));
  }
}

/**
 * @brief LCD1602显示十六进制数字
 * 
 * @param Line    显示起始行位置  
 * @param Column  显示起始列位置
 * @param Number  显示的十六进制数字
 * @param Length  显示十六进制数字的长度（不包括 `0x` 前缀）
 */
void LCD_ShowHexNum(unsigned char Line, Column, unsigned int Number, unsigned char Length) {
  unsigned char i = 0;
  unsigned char SingleNumber = 0;
  LCD_SetCursor(Line, Column);
  for (i = Length; i > 0; --i) {
    SingleNumber = Number / LCD_Pow(16, i - 1) % 16;
    if (SingleNumber < 10) {
      LCD_WriteData('0' + SingleNumber);
    } else {
      LCD_WriteData('A' + SingleNumber - 10);
    } 
  }
}

/**
 * @brief LCD1602显示二进制数字
 * 
 * @param Line    显示起始行位置
 * @param Column  显示起始列位置
 * @param Number  显示的二进制数字
 * @param Length  显示二进制数字的长度（不包括 `b` 或 `B` 后缀）
 */
void LCD_ShowBinNum(unsigned char Line, Column, unsigned int Number, unsigned char Length) {
  unsigned char i;
  LCD_SetCursor(Line, Column);
  for (i = Length; i > 0; --i) {
    LCD_WriteData('0' + (Number / LCD_Pow(2, i - 1) % 2));
  }
}