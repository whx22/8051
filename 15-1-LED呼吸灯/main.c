#include <REGX52.H>

sbit LED = P2^0;

void Delay(unsigned int t) {
  while (t--); // �˴�t--��--tЧ����ͬ
}

void main() {
  unsigned char Time, i;
  while (1) {
    for (Time = 0; Time < 100; ++Time) { // ���ȱ仯(����)
      for (i = 0; i < 20; ++i) { // ��ʱ
        LED = 0;  // LED��
        Delay(Time);
        LED = 1;  // LED��
        Delay(100 - Time);
      }
    }
    for (Time = 100; Time > 0; --Time) { // ���ȱ仯��������
      for (i = 0; i < 20; ++i) { // ��ʱ
        LED = 0; // LED��
        Delay(Time);
        LED = 1; // LED��
        Delay(100 - Time);
      }
    }
  }
}