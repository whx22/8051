// 延时函数单位ms(STC-ISP生成) @12.000MHz
void Delay(unsigned int xms) {
	unsigned char i, j;
	while(xms--) {
		i = 2;
		j = 239;
		do {
			while (--j);
		} while (--i);
	}
}