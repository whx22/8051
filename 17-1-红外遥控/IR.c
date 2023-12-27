#include <REGX52.H>
#include "Timer0.h"
#include "Int0.h"

// ��¼����ߵ�ƽʱ�䣬����NECЭ�������Ԫ��Start, Logical 0, logical 1, Repeat�� 
unsigned int IR_Time;
// ��¼���ܺ����źŹ���״̬��
// 0���У��ȴ��½��ش����ⲿ�жϽ���״̬1
// 1ʱ���жϣ�Start Repeat Invalid
// 2�������ݣ�����4��8λ��32λ����
//  �����ⷢ������ַ�룬���ⷢ������ַ��ķ���
//    �����룬������ķ��룩
unsigned char IR_State;

// ���ܺ����źŻ���
unsigned char IR_Data[4];
// �źŻ���λ����
unsigned char IR_pData;

// ���ݽ�����ɱ�־λ
unsigned char IR_DataFlag;
// �����ظ����ͱ�־λ
unsigned char IR_RepeatFlag;
// ���ܵ��ĵ�ַ���ݺ���������
unsigned char IR_Address;
unsigned char IR_Command;

/**
 * @brief �����źŽ�������ʼ��
 * 
 */
void IR_Init(void) {
  Timer0_Init();
  Int0_Init();
}

/**
 * @brief ��Ƭ����ȡ�����źŽ��������ܵ����ݱ�־λ
 * 
 * @return unsigned char �Ƿ���ܵ�����
 */
unsigned char IR_GetDataFlag(void) {
  if (IR_DataFlag) {
    IR_DataFlag = 0;
    return 1;
  }
  return 0;
}

/**
 * @brief ��Ƭ����ȡ�����źŽ��������ܵ��ظ���־λ
 * 
 * @return unsigned char �Ƿ���ܵ��ظ��ź�
 */
unsigned char IR_GetRepeatFlag(void) {
  if (IR_RepeatFlag) {
    IR_RepeatFlag = 0;
    return 1;
  }
  return 0;
}

/**
 * @brief ���ܵ������źŻ��ظ��źź󣬻�ȡ���ⷢ������ַ���ݣ�NECЭ��涨��
 * 
 * @return unsigned char ���ⷢ������ַ����
 */
unsigned char IR_GetAddress(void) {
  return IR_Address;
}

/**
 * @brief ���ܵ������źŻ��ظ��źź󣬻�ȡ���ⷢ������������
 * 
 * @return unsigned char ���ⷢ�������ܵ�����������
 */
unsigned char IR_GetCommand(void) {
  return IR_Command;
}

/**
 * @brief ����������������ⲿ�ж�0���жϴ�����
 *        �ⲿ�ж�0��ʼ������������Ϊ�½��ش���
 */
void Int0_Rountine(void) interrupt 0 {
// ״̬0������״̬תΪ�ж�״̬
  if (IR_State == 0) {
    Timer0_SetCounter(0);
    Timer0_Run(1);
    IR_State = 1;
  } else if (IR_State == 1) {
// ״̬1���ж�״̬
    IR_Time = Timer0_GetCounter(); // ��ȡ���ʱ�䣬�����ж�
    Timer0_SetCounter(0); // ���ö�ʱ��0
    if (IR_Time > 13500-500 && IR_Time < 13500+500) { // NEC����Start
      P2 = 0;
      IR_State = 2; // �������ݴ���״̬
    } else if (IR_Time > 11250-500 && IR_Time < 11250+500) { // NEC����Repeat
      IR_RepeatFlag = 1; // �����ظ���־λ
      Timer0_Run(0); // �رն�ʱ��
      IR_State = 0; // �������״̬
    } else {
      IR_State = 1; // ������󣬻ָ��ж�״̬
    }
  } else if (IR_State == 2) {
// ״̬2�����ݴ���״̬
    IR_Time = Timer0_GetCounter();
    Timer0_SetCounter(0);
    if (IR_Time > 1120-500 && IR_Time < 1120-500) { // NEC����0
      // �ӵ�λ����λ��32λ���ν�������ݴ����źŻ���
      IR_Data[IR_pData / 8] &= ~(0x01 << (IR_pData % 8));
    } else if (IR_Time > 2250-500 && IR_Time < 2250-500) { // NEC����1
      IR_Data[IR_pData / 8] |= (0x01 << (IR_pData % 8));
    } else { // ����NEC����
      IR_pData = 0; // �������ݽ���״̬
      IR_State = 1; // ����һ���ⲿ�ж�ʱ�����½����ж�״̬
    }
    if (IR_pData >= 32) { // ���ݴ������
      // �����źŻ�����������
      IR_pData = 0;
      // �����ź�У��ɹ�
      if ((IR_Data[0] == ~IR_Data[1]) && (IR_Data[2] == ~IR_Data[3])) {
        IR_Address = IR_Data[0];
        IR_Command = IR_Data[2];
        IR_DataFlag = 1;
      }
      Timer0_Run(0); // ��ͣ��ʱ��0
      IR_State = 0; // ���»ص�״̬0����
    }
  }
}