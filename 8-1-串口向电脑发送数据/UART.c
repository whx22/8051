#include <REGX52.H>

/**
 * @brief  ���ڳ�ʼ�� 4800bps@12.000MHz
 * @param  ��
 * @retval ��
 */
void UART_Init() {
// ����ͨ�ſ��ƼĴ���SCON
	// ����ģʽ1��SM0 = 0 SM1 = 1
	// ���ܲ�ʹ�� REN = 0
	SCON = 0x40;
// ��Դ���ƼĴ���PCON
	// �����ʼӱ�SMOD = 1
	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD = 1	
// ���ö�ʱ��
	TMOD &= 0x0F;		//����timer1ģʽ
	TMOD |= 0x20;		//����timer1ģʽΪ8λ�Զ���װ
	TL1 = 0xF3;		//�趨��ʱ��ֵ
	TH1 = 0xF3;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
}

/**
 * @brief  ��������λ������һ���ֽڵ�����
 * @param  ��Ҫ���͵�һ���ֽ�����
 * @retval ��
 */
void UART_SendByte(unsigned char Byte) {
	SBUF = Byte;
	while(TI == 0);
	TI = 0;
}