#include "timer.h"
#include "task.h"
#include "uart.h"
extern uint32_t task_count;
extern Task_Struct tasks[];

static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // ??????0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// ??????
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn; 	
		// ???????
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ??????
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ??:TIM_TimeBaseInitTypeDef??????5???,TIM6?TIM7????????
 * TIM_Prescaler?TIM_Period,????TIM6?TIM7???????????????,
 * ????????????????????.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ??
 * TIM_CounterMode			 TIMx,x[6,7]??,????(?????)
 * TIM_Period            ??
 * TIM_ClockDivision     TIMx,x[6,7]??,????(?????)
 * TIM_RepetitionCounter TIMx,x[1,8]??(?????)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// ??TIMx_CLK,x[6,7] 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

  /* ?? TIM_Period????????????*/		
  //?????0???4999,??5000?,???????
  TIM_TimeBaseStructure.TIM_Period = 10000-1;       
	
	// ??????????TIMxCLK = HCLK/2=84MHz 
	// ????????=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	
  // ??????
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ????
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// ??????TIMx, x[1,8]
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	
	// ????????????
	TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	
	// ?????????
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
	
	// ?????
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

/**
  * @brief  ????????????,1ms??????
  * @param  ?
  * @retval ?
  */
void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	
  
  TIM_Mode_Config();
}

void TIM2_IRQHandler(void)
{
	uint8_t i = 0;

	if (RESET != TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		for (i = 0; i < task_count; ++i)
		{
			if(tasks[i].TimerSlice)
			{
				--tasks[i].TimerSlice;
				if(0 == tasks[i].TimerSlice)
				{
					tasks[i].isRun = 0x01;
					tasks[i].TimerSlice = tasks[i].SliceNumber;
				}
			}
		}
	}
}

