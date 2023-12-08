#include <REGX52.H>

void Delay(unsigned int xms)
{
	unsigned char i, j;
	while (xms) {
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);	
		xms--;
	}
}


void main() {
	while (1) {
		if (P3_1 == 0) { // 独立按键：按下时低电平（0）
			Delay(20); // 按下：按键按下瞬间消抖
			while (P3_1 == 0); // 检测按键处于按下状态
			Delay(20); // 松开：按键会弹瞬间消抖

			P2_0 = ~P2_0; // LED状态取反
		}
	}
}