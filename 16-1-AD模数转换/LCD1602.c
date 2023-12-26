#include <REGX52.H>

sbit LCD_WR = P2^5; 
sbit LCD_RS = P2^6;
sbit LCD_EN = P2^7;
#define LCD_DataPort P0

/**
 * @brief   LCD�ڲ���ʱ����
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
 * @brief   ��LCD1602д������д���8bit�����֣�
 * 
 * @param Command ��LCD1602�����������
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
 * @brief   ��LCD1602д���ݣ����д���8bit���ݣ�
 * 
 * @param Data ��LCD1602���������
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
 * @brief LCD1602��ʼ��
 * 
 */
void LCD_Init(void) {
  LCD_WriteCommand(0x38); // ��λ���ݽӿڣ�������ʾ��5 * 7����
  LCD_WriteCommand(0x0C); // ��ʾ�������أ���˸��
  LCD_WriteCommand(0x06); // ���ݶ���д������AC�Զ���һ�����治��
  LCD_WriteCommand(0x01); // ���������DDRAM��ACֵ����긴λ
}

/**
 * @brief LCD1602���ù��λ��
 * 
 * @param Line    �����λ��
 * @param Column  �����λ��
 */
void LCD_SetCursor(unsigned char Line, Column) {
  if (Line == 1) {
    LCD_WriteCommand(0x80 | (Column - 1));
  } else {
    LCD_WriteCommand(0x80 | ((Column - 1) + 0x40));
  }
}

/**
 * @brief LCD1602��ʾһ���ַ�
 * 
 * @param Line    �ַ���λ��
 * @param Column  �ַ���λ��
 * @param Char    �ַ�
 */
void LCD_ShowChar(unsigned char Line, Column, char Char) {
  LCD_SetCursor(Line, Column);
  LCD_WriteData(Char);
}

/**
 * @brief LCD1602��ʾһ���ַ���
 * 
 * @param Line    �ַ�����ʼ��λ��
 * @param Column  �ַ�����ʼ��λ��
 * @param String  �ַ���
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
 * @brief ����X^Y
 * 
 * @param X     ����
 * @param Y     ָ��
 * @return int  ֵ
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
 * @brief LCD1602��ʾLengthλ�޷�������
 * 
 * @param Line    ��ʾ��ʼ��λ��
 * @param Column  ��ʾ��ʼ��λ��
 * @param Number  ��ʾ������
 * @param Length  ��ʾ���ֵĳ���
 */
void LCD_ShowNum(unsigned char Line, Column, unsigned int Number, unsigned char Length) {
  unsigned char i;
  LCD_SetCursor(Line, Column);
  for (i = Length; i > 0; --i) {
    LCD_WriteData(0x30 + (Number / LCD_Pow(10, i - 1) % 10));
  }
}

/**
 * @brief LCD1602��ʾһ���з�������
 * 
 * @param Line    ��ʾ��ʼ��λ��
 * @param Column  ��ʾ��ʼ��λ��
 * @param Number  ��ʾ������
 * @param Length  ��ʾ���ֵĳ��ȣ�����������λ��
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
 * @brief LCD1602��ʾʮ����������
 * 
 * @param Line    ��ʾ��ʼ��λ��  
 * @param Column  ��ʾ��ʼ��λ��
 * @param Number  ��ʾ��ʮ����������
 * @param Length  ��ʾʮ���������ֵĳ��ȣ������� `0x` ǰ׺��
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
 * @brief LCD1602��ʾ����������
 * 
 * @param Line    ��ʾ��ʼ��λ��
 * @param Column  ��ʾ��ʼ��λ��
 * @param Number  ��ʾ�Ķ���������
 * @param Length  ��ʾ���������ֵĳ��ȣ������� `b` �� `B` ��׺��
 */
void LCD_ShowBinNum(unsigned char Line, Column, unsigned int Number, unsigned char Length) {
  unsigned char i;
  LCD_SetCursor(Line, Column);
  for (i = Length; i > 0; --i) {
    LCD_WriteData('0' + (Number / LCD_Pow(2, i - 1) % 2));
  }
}