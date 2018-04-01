#ifndef _TASK_H
#define _TASK_H

#include "stm32f4xx.h"

void Task_Process(void);
void Task1(void * parameter);
void Task2(void * parameter);
void Task3(void * parameter);
void Task4(void * parameter);

typedef struct _Task_Struct
{
	uint8_t isRun;						//��ʾ�����Ƿ�����
	uint16_t TimerSlice;			//����������ʱ��Ƭ
	uint16_t SliceNumber;		//ʱ��Ƭ�ĸ�������TimerSliceΪ0ʱ�����丳ֵ��TimerSlice���¼���
	void (*TaskPointer)(void* parameter);		//����ĺ���ָ��
}Task_Struct, *Task_Struct_Pointer;

#endif
