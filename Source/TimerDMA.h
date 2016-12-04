/*
 * TimerDMA.h
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */

#ifndef TIMER_DMA_H_
#define TIMER_DMA_H_


//*****************************************************************
//          CPWM
//*****************************************************************
//the ADC driver handles reading the enabled channels using DMA and managing the last N results
class CTimerDMA
{
protected:
public:
	CTimerDMA(unsigned int nFrequency = 10000):m_nFrequency(nFrequency)
    {
    }
    void Init();
    unsigned int m_nTimerReloadRegister;
    unsigned int m_nFrequency;
};



#endif /* PWM_H_ */
