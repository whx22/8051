#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"
#include "DS18B20.h"

float T;

void main() {
    // 高精度温度转化需要时延，
    // 延时1ms，防止第一次读出DS18B20默认值
    DS18B20_ConvertT();
    Delay(1000);
    
    LCD_Init();
    LCD_ShowString(1, 1, "Temperature:");
    while (1) {
        DS18B20_ConvertT(); // 温度转化
        T = DS18B20_ReadT(); // 温度读取
        // 显示符号位
        if (T < 0) {
            LCD_ShowChar(2, 1, '-');
            T = -T;
        } else {
            LCD_ShowChar(2, 1, '+');
        }
        LCD_ShowNum(2, 2, T, 3); // 显示整数部分
        LCD_ShowChar(2, 5, '.'); // 显示小数点
        LCD_ShowNum(2, 6, (unsigned long)(T * 10000) % 10000, 4); // 显示小数部分
    } 
}