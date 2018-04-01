#include "task.h"
#include "uart.h"
#include "adc.h"
#include <stdio.h>

Task_Struct tasks[]={
{0, 200, 200, Task1},
{0, 60, 60, Task2},
{0, 100, 100, Task3},
{0, 10, 10, Task4}
};
u32 task_count = sizeof(tasks)/ sizeof(Task_Struct);

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[2];

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal[2]; 

void Task1(void * parameter)
{
	Usart_SendString(DEBUG_USART, "led is blinking.\r\n");
}

void Task2(void * parameter)
{
	Usart_SendString(DEBUG_USART, "lcd is running.\r\n");
}

void Task3(void * parameter)
{
	Usart_SendString(DEBUG_USART, "A File is downloading.\r\n");
}

void Task4(void * parameter)
{

	//Usart_SendString(DEBUG_USART, "A Key is pressing.\r\n");
	  ADC_ConvertedValueLocal[0] =(float)((uint16_t)ADC_ConvertedValue[0]*3.3/0.68/4096); 
    ADC_ConvertedValueLocal[1] =(float)((uint16_t)ADC_ConvertedValue[1]*3.3/0.68/4096);    
    
    printf("\r\n The current AD value = 0x%08X \r\n", ADC_ConvertedValue[0]); 
    printf("\r\n The current AD value = 0x%08X \r\n", ADC_ConvertedValue[1]);     
    
    printf("\r\n The current ADC1 value = %f V \r\n",ADC_ConvertedValueLocal[0]); 
    printf("\r\n The current ADC2 value = %f V \r\n",ADC_ConvertedValueLocal[1]); 
}


void Task_Process()
{
	u8 i = 0;
	for (i = 0; i < task_count; ++i)
	{
		if (tasks[i].isRun)
		{
			tasks[i].TaskPointer(NULL);
			tasks[i].isRun = 0;
		}
	}
}
