/*
 * DAC.cc
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */

#include "DAC.h"
#include "misc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dac.h"

//*****************************************************************
//          CDAC
//*****************************************************************
void CDAC::Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    DAC_InitTypeDef  DAC_InitStructure;
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);

    DAC_Cmd(DAC_Channel_1, ENABLE);
}

//0 to 3.3 floating point value
void CDAC::SetDAC(float fValue)
{
    uint16_t nData = fValue * 4095 / 3.3;
    if(nData > 4095) nData = 4095;
    DAC_SetChannel1Data(DAC_Align_12b_R, nData);
}
