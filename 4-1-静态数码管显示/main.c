#include <REGX52.H>

// 数码管对应数字对应的段码表
unsigned char NixieTable[] = \
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
// 0     1     2     3     4     5     6     7     8     9

/*
 * 描述：数码管显示
 * 参数1：Location: 指定显示的数码管
 * 参数2：Number: 指定数码管显示的数字
 */
void Nixie(unsigned char Location, unsigned char Number) {
	// 根据具体Location参数，选中对应数码管
	switch(Location) {
		case 1: P2_4 = 1, P2_3 = 1, P2_2 = 1; break; // 第一个数码管：LED8，Y7 
		case 2: P2_4 = 1, P2_3 = 1, P2_2 = 0; break; 
		case 3: P2_4 = 1, P2_3 = 0, P2_2 = 1; break;
		case 4: P2_4 = 1, P2_3 = 0, P2_2 = 0; break;
		case 5: P2_4 = 0, P2_3 = 1, P2_2 = 1; break;
		case 6: P2_4 = 0, P2_3 = 1, P2_2 = 0; break;
		case 7: P2_4 = 0, P2_3 = 0, P2_2 = 1; break;
		case 8: P2_4 = 0, P2_3 = 0, P2_2 = 0; break;
	}
	// 根据Number参数显示对应数字
	P0 = NixieTable[Number];
}

void main() {
	// 位选LED6数码管
	P2_4 = 1;
	P2_3 = 0;
	P2_2 = 1;

	// 段码显示6,高位对高位，低位对低位
	P0 = 0x7D;

	// 使用函数Nixie进行数码管显示
	Nixie(2, 3);
	while (1) {
		
	}
}