#include <REGX52.h>
#include "Delay.h"

/**
 * @brief  获取独立按键键码
 * @param  void
 * @retval 独立按键对应键码，范围0~4，无按键：0
 */
unsigned char Key() {
	unsigned char KeyNumber = 0;

	if (P3_1 == 0) {Delay(20); while(P3_1 == 0); Delay(20); KeyNumber = 1;}
	if (P3_0 == 0) {Delay(20); while(P3_0 == 0); Delay(20); KeyNumber = 2;}
	if (P3_2 == 0) {Delay(20); while(P3_2 == 0); Delay(20); KeyNumber = 3;}
	if (P3_3 == 0) {Delay(20); while(P3_3 == 0); Delay(20); KeyNumber = 4;}

	return KeyNumber;
}