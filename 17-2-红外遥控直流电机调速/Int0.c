#include <REGX52.H>

void Int0_Init(void) {
  // �����ⲿ�ж�
  IT0 = 1;  // �ⲿ�жϴ�����ʽ��1�½��ش�����0�͵�ƽ���� 
  IE0 = 0;  // �ⲿ�жϱ�־����
  EX0 = 1;  // �����ⲿ�ж�0
  EA = 1;   // ���������ж�
  PX0 = 1;  // ���ж����ȼ�
}

/*
void Int0_Rountine(void) interrupt 0 {
  
}
*/
