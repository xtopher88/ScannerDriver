/*
 * PWM.h
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */

#ifndef PWM_H_
#define PWM_H_

//*****************************************************************
//          CPWM
//*****************************************************************
//the PWM
class CPWM
{
protected:
public:
    CPWM(unsigned int nFrequency = 100000):m_nFrequency(nFrequency)
    {
    }
    void Init();
    unsigned int m_nTimerReloadRegister;
    unsigned int m_nFrequency;
    void SetChannel1(float fDuty);
    void SetChannel2(float fDuty);
    void SetChannel3(float fDuty);
    void SetChannel4(float fDuty);
    void SetChannel1(unsigned int nPulse);
    void SetChannel2(unsigned int nPulse);
    void SetChannel3(unsigned int nPulse);
    void SetChannel4(unsigned int nPulse);
};



#endif /* PWM_H_ */
