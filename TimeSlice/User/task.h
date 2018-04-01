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
	uint8_t isRun;						//表示任务是否运行
	uint16_t TimerSlice;			//分配给任务的时间片
	uint16_t SliceNumber;		//时间片的个数，当TimerSlice为0时，将其赋值给TimerSlice重新计数
	void (*TaskPointer)(void* parameter);		//任务的函数指针
}Task_Struct, *Task_Struct_Pointer;

#endif
