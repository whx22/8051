#include <REGX52.H>

sbit LCD_WR = P2^5; 
sbit LCD_RS = P2^6;
sbit LCD_EN = P2^7;
#define LCD_DataPort P0

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

void LCD_WriteCommand(unsigned char Command) {
  LCD_RS = 0;
  LCD_WR = 0;
  LCD_DataPort = Command;
  LCD_EN = 1;
  LCD_Delay();
  LCD_EN = 0;
  LCD_Delay();
}

void LCD_WriteData(unsigned char Data) {
  LCD_RS = 1;
  LCD_WR = 0;
  LCD_DataPort = Data;
  LCD_EN = 1;
  LCD_Delay();
  LCD_EN = 0;
  LCD_Delay();
}

void LCD_Init(void) {
  LCD_WriteCommand(0x38);
  LCD_WriteCommand(0x0C);
  LCD_WriteCommand(0x06);
  LCD_WriteCommand(0x01);
}

void LCD_SetCursor(unsigned char Line, Column) {
  if (Line == 1) {
    LCD_WriteCommand(0x80 | (Column - 1));
  } else {
    LCD_WriteCommand(0x80 | ((Column - 1) + 0x40));
  }
}

void LCD_ShowChar(unsigned char Line, Column, char Char) {
  LCD_SetCursor(Line, Column);
  LCD_WriteData(Char);
}

void LCD_ShowString(unsigned char Line, Column, char *String) {
  unsigned char i = 0;
  LCD_SetCursor(Line, Column);
  while (String[i] != '\0') {
    LCD_WriteData(String[i]);
    ++i;
  }
}

int LCD_Pow(int X, int Y) {
  unsigned char i;
  int Result = 1;
  for (i = 0; i < Y; ++i) {
    Result *= X;
  }
  return Result;
}

void LCD_ShowNum(unsigned char Line, Column, unsigned int Number, unsigned char Length) {
  unsigned char i;
  LCD_SetCursor(Line, Column);
  for (i = Length; i > 0; --i) {
    LCD_WriteData(0x30 + (Number / LCD_Pow(10, i - 1) % 10));
  }
}

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

void LCD_ShowBinNum(unsigned char Line, Column, unsigned int Number, unsigned char Length) {
  unsigned char i;
  LCD_SetCursor(Line, Column);
  for (i = Length; i > 0; --i) {
    LCD_WriteData('0' + (Number / LCD_Pow(2, i - 1) % 2));
  }
}