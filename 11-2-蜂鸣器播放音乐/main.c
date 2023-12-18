#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"

sbit Buzzer = P2^5;

// 定义4分音符时长，单位ms
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

// C自然大调小字组，小字1组，小字2组
unsigned int FrequenceTable[] = {
	0, // 休止符 无声音
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};

// 音符+时长，时长基本单位 16分音符
// 16分音符	1
// 8分音符	2
// 4分音符 	4
// 2分音符	8
// 全音符	16

// 小星星
unsigned char Music_Star[] = {
	13, 4, 13, 4, 20, 4, 20, 4,
	22, 4, 22, 4, 20, 8,
	0, 4,
	18, 4, 18, 4, 17, 4, 17, 4,
	15, 4, 15, 4, 13, 8,
	0xFF // 音乐终止标志
};

unsigned char FrequenceSelect, MusicSelect;

void main() {
	Timer0Init();
	while (1) {
		if (Music_Star[MusicSelect] != 0xFF) {
			FrequenceSelect = Music_Star[MusicSelect]; // 当前Music数组数据为音符频率对应IO翻转周期
	  		++MusicSelect;
			Delay(SPEED / 4 * Music_Star[MusicSelect]); // 当前Music数组数据为音符持续时长
	  		++MusicSelect;
			// 实现相同音的间隔 
			TR0 = 0; // 关闭定时器
			Delay(5);
			TR0 = 1; // 开启定时器
		} else {
			// 音乐播放结束，关闭音乐 or 重新播放
			TR0 = 0; // 关闭音乐：关闭定时器
			//MusicSelect = 0; // 重新播放
			while (1);
		}

	}
}

void Timer0_Routine() interrupt 1 { 
	if (FrequenceTable[FrequenceSelect]) { // 休止符时不振荡
		TL0 = FrequenceTable[FrequenceSelect] % 256;		//设置定时初值
		TH0 = FrequenceTable[FrequenceSelect] / 256;		//设置定时初值
		Buzzer = !Buzzer;
	}
}