#include <REGX52.H>

// λ��������Ƭ����DS1302оƬ���ӵĶ˿ڶ���
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;

// ���ʱ�����ݵ����飨��Ƭ����������EEPROM��
char DS1302_Time[] = {
	23, // year
	12, // month
	17,	// date
	16,	// hour
	59,	// minute
	55,	// second
	6	// day
};

// DS1302д�������ֵ�ַ
#define DS1302_SECOND 	0x80
#define DS1302_MINUTE 	0x82
#define DS1302_HOUR		0x84 	
#define DS1302_DATE		0x86
#define DS1302_MONTH	0x88
#define DS1302_DAY		0x8A
#define DS1302_YEAR		0x8C
#define DS1302_WP 		0x8E

/**
 * @brief	DS1302оƬ��ʼ��
 * @param 	��
 * @retval	��
 */
void DS1302_Init(void) {
	DS1302_CE = 0;
	DS1302_SCLK = 0;  
}

/**
 * @brief 	��Ƭ����DS1302оƬд������ 
 * @param1 	д�������֣�ָ����DS1302оƬд��ļĴ�����
 * @param2	д������
 * @retval	��
 * @note 	DS1302_SCLK ����16�������źţ�
 			16���������ź�
 */
void DS1302_WriteByte(unsigned char Command, Data) {
	unsigned char i = 0;
// DS1302оƬ��ʼ���ݽ��ܣ�����16������
	DS1302_CE = 1;

// DS1302����������
	for (i = 0; i < 8; ++i) {
		DS1302_IO = Command & (0x01 << i);
		// ����һ��SCLK�����أ�
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
// DS1302��������
	for (i = 0; i < 8; ++i) {
		DS1302_IO = Data & (0x01 << i);
		// ����һ��SCLK�����أ�
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
// DS1302оƬ�������ݽ���
	DS1302_CE = 0;
}

/**
 * @brief	��Ƭ����DS1302оƬ��������
 * @param	���������֣�ָ����DS1302оƬ�����ļĴ�����
 * @retval	����������
 * @note 	DS1302_SCLK ����15�������źţ�
 			8���������źţ�8���½����źţ�
			���д���һ���������źź��½����źŹ���һ�������ź�
 */
unsigned char DS1302_ReadByte(unsigned char Command) {
	unsigned char i = 0, Data = 0x00;

	Command |= 0x01; // DS1302д�������ֵ�ַת��Ϊ���������ֵ�ַ
// DS1302оƬ��ʼ�������������15������
	DS1302_CE = 1;

// DS1302����������
	for (i = 0; i < 8; ++i) {
		DS1302_IO = Command & (0x01 << i);
		// ����һ��SCLK�����أ�
		DS1302_SCLK = 0;
		DS1302_SCLK = 1;
	}
// DS1302�������
	for (i = 0; i < 8; ++i) {
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
		if (DS1302_IO) {
			Data |= (0x01 << i);
		}
	}
// DS1302оƬ�����������
	DS1302_CE = 0;
	DS1302_IO = 0;
	return Data;
}

/**
 * @brief	ʹ�õ�Ƭ���е�ʱ����������DS1302оƬ�洢��ʱ��
 * @param	��
 * @retval	��
 * @note	DS1302��ʱ��ʹ��BCD���룬
 			ʮ����(DEC)����תΪBCD����
			BCD = DEC / 10 * 16 + DEC % 10;

 */
void DS1302_SetTime(void) {
	// �ر�д����
	DS1302_WriteByte(DS1302_WP, 0x00);
	DS1302_WriteByte(DS1302_YEAR, DS1302_Time[0] / 10 * 16 + DS1302_Time[0] % 10);
	DS1302_WriteByte(DS1302_MONTH, DS1302_Time[1] / 10 * 16 + DS1302_Time[1] % 10);
	DS1302_WriteByte(DS1302_DATE, DS1302_Time[2] / 10 * 16 + DS1302_Time[2] % 10);
	DS1302_WriteByte(DS1302_HOUR, DS1302_Time[3] / 10 * 16 + DS1302_Time[3] % 10);
	DS1302_WriteByte(DS1302_MINUTE, DS1302_Time[4] / 10 * 16 + DS1302_Time[4] % 10);
	DS1302_WriteByte(DS1302_SECOND, DS1302_Time[5] / 10 * 16 + DS1302_Time[5] % 10);
	DS1302_WriteByte(DS1302_DAY, DS1302_Time[6] / 10 * 16 + DS1302_Time[6] % 10);
	// ��д����
	DS1302_WriteByte(DS1302_WP, 0x80);
}

/**
 * @brief  	��ȡDS1302�д洢��ʱ�䲢���õ���Ƭ����ʱ��������	
 * @param  	��
 * @retval	��
 * @note	DS1302��ʱ��ʹ��BCD���룬
 			BCD����תΪʮ����(DEC)����
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