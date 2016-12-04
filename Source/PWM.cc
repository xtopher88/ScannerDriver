/*
 * PWM.cc
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */


#include "PWM.h"

#include "stm32f4xx_conf.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

//*****************************************************************
//          CPWM
//*****************************************************************
//the ADC driver handles reading the enabled channels using DMA and managing the last N results
void CPWM::Init()
{
    //Compute the prescaler and reload register value
    RCC_ClocksTypeDef RCC_ClocksStatus;
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    uint32_t nPCLKFrequency = RCC_ClocksStatus.PCLK1_Frequency << 1;
    unsigned int count = nPCLKFrequency / m_nFrequency - 1;
    unsigned int prescale = count / 65536;
    m_nTimerReloadRegister = count/(prescale+1);

    //enable clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //Time base configuration
    TIM_TimeBaseInitTypeDef TimeBaseStructure;
    TIM_TimeBaseStructInit( &TimeBaseStructure );
    TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TimeBaseStructure.TIM_Period = m_nTimerReloadRegister;
    TimeBaseStructure.TIM_Prescaler = prescale; // 0 is a prescale of 1
    TIM_TimeBaseInit(TIM4, &TimeBaseStructure);


    //TODO enable GPIOs on AF
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);

    //PWM4 Mode configuration
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

    //PWM4 Mode configuration: Channel1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    //PWM4 Mode configuration: Channel2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    //PWM4 Mode configuration: Channel3
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    //PWM4 Mode configuration: Channel4
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
    //TIM_CtrlPWMOutputs(TIM4, ENABLE);
}


void CPWM::SetChannel1(float fDuty)
{
    if(fDuty > 100.0) fDuty = 100.0;
    float fTemp = fDuty * m_nTimerReloadRegister;
    unsigned int nPulse = fTemp / 100.0;
    TIM_SetCompare1(TIM4,nPulse);
}

void CPWM::SetChannel2(float fDuty)
{
    if(fDuty > 100.0) fDuty = 100.0;
    float fTemp = fDuty * m_nTimerReloadRegister;
    unsigned int nPulse = fTemp / 100.0;
    TIM_SetCompare2(TIM4,nPulse);
}

void CPWM::SetChannel3(float fDuty)
{
    if(fDuty > 100.0) fDuty = 100.0;
    float fTemp = fDuty * m_nTimerReloadRegister;
    unsigned int nPulse = fTemp / 100.0;
    TIM_SetCompare3(TIM4,nPulse);
}

void CPWM::SetChannel4(float fDuty)
{
    if(fDuty > 100.0) fDuty = 100.0;
    float fTemp = fDuty * m_nTimerReloadRegister;
    unsigned int nPulse = fTemp / 100.0;
    TIM_SetCompare4(TIM4,nPulse);
}


void CPWM::SetChannel1(unsigned int nPulse)
{
    TIM_SetCompare1(TIM4,nPulse);
}

void CPWM::SetChannel2(unsigned int nPulse)
{
    TIM_SetCompare2(TIM4,nPulse);
}

void CPWM::SetChannel3(unsigned int nPulse)
{
    TIM_SetCompare3(TIM4,nPulse);
}

void CPWM::SetChannel4(unsigned int nPulse)
{
    TIM_SetCompare4(TIM4,nPulse);
}

