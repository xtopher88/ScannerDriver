/*
 * TimerDMA.cc
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */


#include "TimerDMA.h"

#include "stm32f4xx_conf.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"


void CTimerDMA::Init()
{
    //Compute the prescaler and reload register value
    RCC_ClocksTypeDef RCC_ClocksStatus;
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    uint32_t nPCLKFrequency = RCC_ClocksStatus.PCLK1_Frequency << 1;
    unsigned int count = nPCLKFrequency / m_nFrequency - 1;
    unsigned int prescale = count / 65536;
    m_nTimerReloadRegister = count/(prescale+1);

    //enable clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //Time base configuration
    TIM_TimeBaseInitTypeDef TimeBaseStructure;
    TIM_TimeBaseStructInit( &TimeBaseStructure );
    TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TimeBaseStructure.TIM_Period = m_nTimerReloadRegister;
    TimeBaseStructure.TIM_Prescaler = prescale; // 0 is a prescale of 1
    TIM_TimeBaseInit(TIM2, &TimeBaseStructure);


    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

    //TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    //TIM_CtrlPWMOutputs(TIM4, ENABLE);
}
