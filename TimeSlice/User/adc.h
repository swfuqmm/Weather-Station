#ifndef _ADC_H
#define	_ADC_H

#include "stm32f4xx.h"

/*=====================ͨ��1 IO======================*/
// PB0 ͨ����ñ�ӵ�λ��
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT1    GPIOB
#define RHEOSTAT_ADC_GPIO_PIN1     GPIO_Pin_0
#define RHEOSTAT_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOB

// ADC ��ź궨��
#define RHEOSTAT_ADC1             ADC1
#define RHEOSTAT_ADC1_CLK         RCC_APB2Periph_ADC1
#define RHEOSTAT_ADC_CHANNEL1     ADC_Channel_8
/*=====================ͨ��2 IO ======================*/
// PB1 ͨ����ñ�ӹ�������
// ADC IO�궨��
#define RHEOSTAT_ADC_GPIO_PORT2    GPIOB
#define RHEOSTAT_ADC_GPIO_PIN2     GPIO_Pin_1
#define RHEOSTAT_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOB

#define RHEOSTAT_ADC2             ADC2
#define RHEOSTAT_ADC2_CLK         RCC_APB2Periph_ADC2
#define RHEOSTAT_ADC_CHANNEL2     ADC_Channel_9

// ADC CDR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_CDR_ADDR    ((uint32_t)0x40012308)

// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0


void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



