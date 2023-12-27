#include <REGX52.H>
#include "Timer0.h"
#include "Int0.h"

// 记录红外高电平时间，区分NEC协议编码码元（Start, Logical 0, logical 1, Repeat） 
unsigned int IR_Time;
// 记录接受红外信号工作状态：
// 0空闲：等待下降沿触发外部中断进入状态1
// 1时序判断：Start Repeat Invalid
// 2接受数据：接受4个8位共32位数据
//  （红外发射器地址码，红外发射器地址码的反码
//    命令码，命令码的反码）
unsigned char IR_State;

// 接受红外信号缓冲
unsigned char IR_Data[4];
// 信号缓冲位索引
unsigned char IR_pData;

// 数据接受完成标志位
unsigned char IR_DataFlag;
// 数据重复发送标志位
unsigned char IR_RepeatFlag;
// 接受到的地址数据和命令数据
unsigned char IR_Address;
unsigned char IR_Command;

/**
 * @brief 红外信号接受器初始化
 * 
 */
void IR_Init(void) {
  Timer0_Init();
  Int0_Init();
}

/**
 * @brief 单片机获取红外信号接收器接受到数据标志位
 * 
 * @return unsigned char 是否接受到数据
 */
unsigned char IR_GetDataFlag(void) {
  if (IR_DataFlag) {
    IR_DataFlag = 0;
    return 1;
  }
  return 0;
}

/**
 * @brief 单片机获取红外信号接收器接受到重复标志位
 * 
 * @return unsigned char 是否接受到重复信号
 */
unsigned char IR_GetRepeatFlag(void) {
  if (IR_RepeatFlag) {
    IR_RepeatFlag = 0;
    return 1;
  }
  return 0;
}

/**
 * @brief 接受到数据信号或重复信号后，获取红外发射器地址数据（NEC协议规定）
 * 
 * @return unsigned char 红外发射器地址编码
 */
unsigned char IR_GetAddress(void) {
  return IR_Address;
}

/**
 * @brief 接受到数据信号或重复信号后，获取红外发射器命令数据
 * 
 * @return unsigned char 红外发射器接受到的命令数据
 */
unsigned char IR_GetCommand(void) {
  return IR_Command;
}

/**
 * @brief 红外接受器触发的外部中断0的中断处理函数
 *        外部中断0初始化过程中设置为下降沿触发
 */
void Int0_Rountine(void) interrupt 0 {
// 状态0：空闲状态转为判断状态
  if (IR_State == 0) {
    Timer0_SetCounter(0);
    Timer0_Run(1);
    IR_State = 1;
  } else if (IR_State == 1) {
// 状态1：判断状态
    IR_Time = Timer0_GetCounter(); // 获取间隔时间，进行判断
    Timer0_SetCounter(0); // 重置定时器0
    if (IR_Time > 13500-500 && IR_Time < 13500+500) { // NEC编码Start
      P2 = 0;
      IR_State = 2; // 进入数据传输状态
    } else if (IR_Time > 11250-500 && IR_Time < 11250+500) { // NEC编码Repeat
      IR_RepeatFlag = 1; // 设置重复标志位
      Timer0_Run(0); // 关闭定时器
      IR_State = 0; // 进入空闲状态
    } else {
      IR_State = 1; // 检码错误，恢复判断状态
    }
  } else if (IR_State == 2) {
// 状态2：数据传输状态
    IR_Time = Timer0_GetCounter();
    Timer0_SetCounter(0);
    if (IR_Time > 1120-500 && IR_Time < 1120-500) { // NEC编码0
      // 从低位到高位，32位依次将检测数据存入信号缓冲
      IR_Data[IR_pData / 8] &= ~(0x01 << (IR_pData % 8));
    } else if (IR_Time > 2250-500 && IR_Time < 2250-500) { // NEC编码1
      IR_Data[IR_pData / 8] |= (0x01 << (IR_pData % 8));
    } else { // 错误NEC编码
      IR_pData = 0; // 放弃数据接受状态
      IR_State = 1; // 在下一次外部中断时，从新进入判断状态
    }
    if (IR_pData >= 32) { // 数据传输结束
      // 接受信号缓冲索引清零
      IR_pData = 0;
      // 接受信号校验成功
      if ((IR_Data[0] == ~IR_Data[1]) && (IR_Data[2] == ~IR_Data[3])) {
        IR_Address = IR_Data[0];
        IR_Command = IR_Data[2];
        IR_DataFlag = 1;
      }
      Timer0_Run(0); // 暂停定时器0
      IR_State = 0; // 从新回到状态0空闲
    }
  }
}