/*
 * ADC.cc
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */

#include "ADC.h"
#include "misc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_adc.h"

void DMA2_Stream4_IRQHandler(void)
{
    //clear the interrupt and call the ADC interrupt handler
    if(DMA_GetITStatus(DMA2_Stream4, DMA_IT_TCIF4) != RESET)
    {
        DMA_ClearITPendingBit(DMA2_Stream4, DMA_IT_TCIF4);

        //call the ADC interrupt
        if(CADC::m_pSelf != 0)
            CADC::m_pSelf->HandleInterrupt();
    }
}

CADC *CADC::m_pSelf=0;

void CADC::HandleInterrupt()
{
    //TODO: ensure this function is not re entered
    m_pcontroller->ADC_Data_Interrupt(m_DataBuffer[0],
            m_DataBuffer[2],
            m_DataBuffer[4],
            m_DataBuffer[5]);
    //TODO remove the unused position sensor values
}



//*****************************************************************
//          CADC
//*****************************************************************
//the ADC driver handles reading the enabled channels using DMA and managing the last N results
void CADC::Init()
{
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

      DMA_DeInit(DMA2_Stream4);
      DMA_InitTypeDef DMA_InitStructure;
      DMA_InitStructure.DMA_Channel = DMA_Channel_0;
      DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)m_DataBuffer;
      DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR); //0x40012308;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
      DMA_InitStructure.DMA_BufferSize = NUM_ADC_CONVERSIONS;
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
      DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
      DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
      DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
      DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
      DMA_Init(DMA2_Stream4, &DMA_InitStructure);

      //DMA_ITConfig(DMA2_Stream4, DMA_IT_TC, ENABLE);

      NVIC_InitTypeDef NVIC_InitStructure;
      NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream4_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);

      DMA_ITConfig(DMA2_Stream4, DMA_IT_TC, ENABLE);

      DMA_Cmd(DMA2_Stream4, ENABLE);



      GPIO_InitTypeDef GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |
              GPIO_Pin_1|
              GPIO_Pin_2|
              GPIO_Pin_3|
              GPIO_Pin_5|
              GPIO_Pin_6;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
      GPIO_Init(GPIOA, &GPIO_InitStructure);


      ADC_CommonInitTypeDef ADC_CommonInitStructure;
      ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
      ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
      ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
      ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
      ADC_CommonInit(&ADC_CommonInitStructure);


      ADC_InitTypeDef ADC_InitStructure;
      ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
      ADC_InitStructure.ADC_ScanConvMode = ENABLE;
      ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ENABLE;
      ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
      ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
      ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
      ADC_InitStructure.ADC_NbrOfConversion = NUM_ADC_CONVERSIONS;
      ADC_Init(ADC1, &ADC_InitStructure);

      ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28Cycles);
      ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_28Cycles);
      ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_28Cycles);
      ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_28Cycles);
      ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_28Cycles);
      ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_28Cycles);


      ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);



       /* Enable ADC3 DMA */
       ADC_DMACmd(ADC1, ENABLE);

      /* Enable ADC1 */
      ADC_Cmd(ADC1, ENABLE);

      //ADC_SoftwareStartConv(ADC1);

}
