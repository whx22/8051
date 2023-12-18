#include <REGX52.H>
#include <INTRINS.H>

/**
 * @brief	��Ƭ��ͨ��P2^5�˿ڿ���ULN2003D����оƬ�������Ʒ���������
 */
sbit Buzzer = P2^5;

/**
 * @brief	������˽����ʱ��������ʱ500us��Ƶ��2000Hz
 * @param	��
 * @retval 	��
 */
void Buzzer_Delay500us()		//@12.000MHz
{
	unsigned char i;

	_nop_(); // ��ʱһ���������ڣ�head file: INTRINS.H
	i = 247;
	while (--i);
}

/**
 * @brief	�����������Ӧʱ��
 * @param	����������ʱ�䣨��λ��ms��
 * @retval 	��
 */
void Buzzer_Time(unsigned int ms) {
	unsigned int i;
	for (i = 0; i < ms * 2; ++i) { // ��תms * 2�Σ�����������ms(ms)
		Buzzer = !Buzzer;
		Buzzer_Delay500us(); // ÿ��500us��ת1�Σ�����1ms��Ƶ��1000Hz
	}
}