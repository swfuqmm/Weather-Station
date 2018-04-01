/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"
#include "task.h"
#include "timer.h"
#include "adc.h"
#include "rtc.h"
#include "i2c_ee.h"
#include <stdio.h>
/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];
uint8_t I2C_Test(void);

static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	
//??ARM????????
#pragma import(__use_no_semihosting)                             
struct __FILE { 
    int handle; 
}; 

FILE __stdout;          
void _sys_exit(int x) 
{ 
    x = x; 
}


int fputc(int ch, FILE *f){      
    while((USART1->SR&0X40)==0);
    USART1->DR = (u8) ch;      
    return ch;
}
#define  EEP_Firstpage      0x00


/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}


int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files before to branch to application main.
       To reconfigure the default setting of SystemInit() function, 
       refer to system_stm32f4xx.c file */

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  
  /* Add your application code here */
  /* Insert 50 ms delay */
  Delay(5);
  
  /* Output HSE clock on MCO1 pin(PA8) ****************************************/ 
  /* Enable the GPIOA peripheral */ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* Configure MCO1 pin(PA8) in alternate function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* HSE clock selected to output on MCO1 pin(PA8)*/
  RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
  
  
  /* Output SYSCLK/4 clock on MCO2 pin(PC9) ***********************************/ 
  /* Enable the GPIOACperipheral */ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* Configure MCO2 pin(PC9) in alternate function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
    
  /* SYSCLK/4 clock selected to output on MCO2 pin(PC9)*/
  RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO2Div_4);
     
  /* Infinite loop */
   //NVIC_Configuration();  
	Debug_USART_Config();
	Usart_SendString(DEBUG_USART, "hello");

                      
	printf("HCLK_Frequency   = %d\r\n", RCC_Clocks.HCLK_Frequency  );
	printf("PCLK1_Frequency  = %d\r\n", RCC_Clocks.PCLK1_Frequency );
	printf("PCLK2_Frequency  = %d\r\n", RCC_Clocks.PCLK2_Frequency );
	printf("SYSCLK_Frequency = %d\r\n", RCC_Clocks.SYSCLK_Frequency);


	TIMx_Configuration(); 
	Rheostat_Init();
	
	RTC_CLK_Config();
	//RTC_TimeAndDate_Set();
	if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
  {
    /* 设置时间和日期 */
		RTC_TimeAndDate_Set();
  }
  else
  {
    /* 检查是否电源复位 */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n 发生电源复位....\r\n");
    }
    /* 检查是否外部复位 */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n 发生外部复位....\r\n");
    }

    printf("\r\n 不需要重新配置RTC....\r\n");
    
    /* 使能 PWR 时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
    PWR_BackupAccessCmd(ENABLE);
    /* 等待 RTC APB 寄存器同步 */
    RTC_WaitForSynchro();   
  } 

	
	I2C_EE_Init();
 	 
	if(I2C_Test() ==1)
	{
		//	LED_GREEN;
	}
	else
	{
		//	LED_RED;
	}

  while (1)
  {	
	//	Task_Process();	
		Delay(0xff);  
	//	RTC_TimeAndData_Printf();
  }
}


/**
  * @brief  I2C(AT24C02)读写测试
  * @param  无
  * @retval 正常返回1 ，不正常返回0
  */
uint8_t I2C_Test(void)
{
	u16 i;

	EEPROM_INFO("写入的数据");
    
	for ( i=0; i<=255; i++ ) //填充缓冲
  {   
    I2c_Buf_Write[i] = i;

    printf("0x%02X ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
        printf("\n\r");    
   }

  //将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);
  
  EEPROM_INFO("写成功");
   
  EEPROM_INFO("读出的数据");
  //将EEPROM读出数据顺序保持到I2c_Buf_Read中
	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 
   
  //将I2c_Buf_Read中的数据通过串口打印
	for (i=0; i<256; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			printf("0x%02X ", I2c_Buf_Read[i]);
			EEPROM_ERROR("错误:I2C EEPROM写入与读出的数据不一致");
			return 0;
		}
    printf("0x%02X ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
        printf("\n\r");
    
	}
  EEPROM_INFO("I2C(AT24C02)读写测试成功");
	return 1;
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
