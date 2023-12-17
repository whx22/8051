#include <REGX52.H>

// 位声明：单片机与DS1302芯片连接的端口定义
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;

// 存放时间数据的数组（单片机数据区，EEPROM）
char DS1302_Time[] = {
	23, // year
	12, // month
	17,	// date
	16,	// hour
	59,	// minute
	55,	// second
	6	// day
};

// DS1302写入命令字地址
#define DS1302_SECOND 	0x80
#define DS1302_MINUTE 	0x82
#define DS1302_HOUR		0x84 	
#define DS1302_DATE		0x86
#define DS1302_MONTH	0x88
#define DS1302_DAY		0x8A
#define DS1302_YEAR		0x8C
#define DS1302_WP 		0x8E

/**
 * @brief	DS1302芯片初始化
 * @param 	无
 * @retval	无
 */
void DS1302_Init(void) {
	DS1302_CE = 0;
	DS1302_SCLK = 0;  
}

/**
 * @brief 	单片机向DS1302芯片写入数据 
 * @param1 	写入命令字，指定向DS1302芯片写入的寄存器号
 * @param2	写入数据
 * @retval	无
 * @note 	DS1302_SCLK 产生16个脉冲信号，
 			16个上升沿信号
 */
void DS1302_WriteByte(unsigned char Command, Data) {
	unsigned char i = 0;
// DS1302芯片开始数据接受，共计16个脉冲
	DS1302_CE = 1;

// DS1302接受命令字
	for (i = 0; i < 8; ++i) {
		DS1302_IO = Command & (0x01 << i);
		// 触发一次SCLK上升沿，
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
// DS1302接受数据
	for (i = 0; i < 8; ++i) {
		DS1302_IO = Data & (0x01 << i);
		// 触发一次SCLK上升沿，
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
// DS1302芯片结束数据接受
	DS1302_CE = 0;
}

/**
 * @brief	单片机从DS1302芯片读出数据
 * @param	读出命令字，指定向DS1302芯片读出的寄存器号
 * @retval	读出的数据
 * @note 	DS1302_SCLK 产生15个脉冲信号，
 			8个上升沿信号，8个下降沿信号，
			其中存在一个上升沿信号和下降沿信号共用一个脉冲信号
 */
unsigned char DS1302_ReadByte(unsigned char Command) {
	unsigned char i = 0, Data = 0x00;

	Command |= 0x01; // DS1302写入命令字地址转化为读出命令字地址
// DS1302芯片开始数据输出，共计15个脉冲
	DS1302_CE = 1;

// DS1302接受命令字
	for (i = 0; i < 8; ++i) {
		DS1302_IO = Command & (0x01 << i);
		// 触发一次SCLK上升沿，
		DS1302_SCLK = 0;
		DS1302_SCLK = 1;
	}
// DS1302输出数据
	for (i = 0; i < 8; ++i) {
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
		if (DS1302_IO) {
			Data |= (0x01 << i);
		}
	}
// DS1302芯片结束数据输出
	DS1302_CE = 0;
	DS1302_IO = 0;
	return Data;
}

/**
 * @brief	使用单片机中的时间数组设置DS1302芯片存储的时间
 * @param	无
 * @retval	无
 * @note	DS1302中时间使用BCD编码，
 			十进制(DEC)编码转为BCD编码
			BCD = DEC / 10 * 16 + DEC % 10;

 */
void DS1302_SetTime(void) {
	// 关闭写保护
	DS1302_WriteByte(DS1302_WP, 0x00);
	DS1302_WriteByte(DS1302_YEAR, DS1302_Time[0] / 10 * 16 + DS1302_Time[0] % 10);
	DS1302_WriteByte(DS1302_MONTH, DS1302_Time[1] / 10 * 16 + DS1302_Time[1] % 10);
	DS1302_WriteByte(DS1302_DATE, DS1302_Time[2] / 10 * 16 + DS1302_Time[2] % 10);
	DS1302_WriteByte(DS1302_HOUR, DS1302_Time[3] / 10 * 16 + DS1302_Time[3] % 10);
	DS1302_WriteByte(DS1302_MINUTE, DS1302_Time[4] / 10 * 16 + DS1302_Time[4] % 10);
	DS1302_WriteByte(DS1302_SECOND, DS1302_Time[5] / 10 * 16 + DS1302_Time[5] % 10);
	DS1302_WriteByte(DS1302_DAY, DS1302_Time[6] / 10 * 16 + DS1302_Time[6] % 10);
	// 打开写保护
	DS1302_WriteByte(DS1302_WP, 0x80);
}

/**
 * @brief  	读取DS1302中存储的时间并设置到单片机中时间数组中	
 * @param  	无
 * @retval	无
 * @note	DS1302中时间使用BCD编码，
 			BCD编码转为十进制(DEC)编码
			DEC = BCD / 16 * 10 + BCD % 16;
 */
void DS1302_ReadTime(void) {
	unsigned char Temp = 0;
	Temp = DS1302_ReadByte(DS1302_YEAR);
	DS1302_Time[0] = Temp / 16 * 10 + Temp % 16;
	Temp = DS1302_ReadByte(DS1302_MONTH);
	DS1302_Time[1] = Temp / 16 * 10 + Temp % 16;
	Temp = DS1302_ReadByte(DS1302_DATE);
	DS1302_Time[2] = Temp / 16 * 10 + Temp % 16;
	Temp = DS1302_ReadByte(DS1302_HOUR);
	DS1302_Time[3] = Temp / 16 * 10 + Temp % 16;
	Temp = DS1302_ReadByte(DS1302_MINUTE);
	DS1302_Time[4] = Temp / 16 * 10 + Temp % 16;
	Temp = DS1302_ReadByte(DS1302_SECOND);
	DS1302_Time[5] = Temp / 16 * 10 + Temp % 16;
	Temp = DS1302_ReadByte(DS1302_DAY);
	DS1302_Time[6] = Temp / 16 * 10 + Temp % 16;
}