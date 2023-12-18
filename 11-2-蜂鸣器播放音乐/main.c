#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"

sbit Buzzer = P2^5;

// ����4������ʱ������λms
#define SPEED 500

#define P 	0
#define L1	1
#define L1_	2
#define L2	3
#define L2_	4
#define L3	5
#define L4	6
#define L4_	7
#define L5	8
#define L5_	9
#define L6	10
#define L6_	11
#define L7	12 
#define M1	13
#define M1_	14
#define M2	15
#define M2_	16
#define M3	17
#define M4	18
#define M4_	19
#define M5	20
#define M5_	21
#define M6	22
#define M6_	23
#define M7	24  
#define H1	25
#define H1_	26
#define H2	27
#define H2_	28
#define H3	29
#define H4	30
#define H4_	31
#define H5	32
#define H5_	33
#define H6	34
#define H6_	35
#define H7	36  

// C��Ȼ���С���飬С��1�飬С��2��
unsigned int FrequenceTable[] = {
	0, // ��ֹ�� ������
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};

// ����+ʱ����ʱ��������λ 16������
// 16������	1
// 8������	2
// 4������ 	4
// 2������	8
// ȫ����	16

// С����
unsigned char Music_Star[] = {
	13, 4, 13, 4, 20, 4, 20, 4,
	22, 4, 22, 4, 20, 8,
	0, 4,
	18, 4, 18, 4, 17, 4, 17, 4,
	15, 4, 15, 4, 13, 8,
	0xFF // ������ֹ��־
};

unsigned char FrequenceSelect, MusicSelect;

void main() {
	Timer0Init();
	while (1) {
		if (Music_Star[MusicSelect] != 0xFF) {
			FrequenceSelect = Music_Star[MusicSelect]; // ��ǰMusic��������Ϊ����Ƶ�ʶ�ӦIO��ת����
	  		++MusicSelect;
			Delay(SPEED / 4 * Music_Star[MusicSelect]); // ��ǰMusic��������Ϊ��������ʱ��
	  		++MusicSelect;
			// ʵ����ͬ���ļ�� 
			TR0 = 0; // �رն�ʱ��
			Delay(5);
			TR0 = 1; // ������ʱ��
		} else {
			// ���ֲ��Ž������ر����� or ���²���
			TR0 = 0; // �ر����֣��رն�ʱ��
			//MusicSelect = 0; // ���²���
			while (1);
		}

	}
}

void Timer0_Routine() interrupt 1 { 
	if (FrequenceTable[FrequenceSelect]) { // ��ֹ��ʱ����
		TL0 = FrequenceTable[FrequenceSelect] % 256;		//���ö�ʱ��ֵ
		TH0 = FrequenceTable[FrequenceSelect] / 256;		//���ö�ʱ��ֵ
		Buzzer = !Buzzer;
	}
}