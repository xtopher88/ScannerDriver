/*
 * ADC.h
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx_conf.h"
#include "stm32f4xx_rcc.h"
#include "Include.h"

#include "Control.h"


#ifdef __cplusplus
 extern "C" {
#endif

void DMA2_Stream4_IRQHandler(void);

#ifdef __cplusplus
}
#endif



//*****************************************************************
//          CADC
//*****************************************************************
//the ADC driver handles reading the enabled channels using DMA and managing the last N results
class CADC
{
public:
    CADC(ControlManager *pcontroller):m_pcontroller(pcontroller)
    {
        m_pSelf = this;
        for(int x=0;x<NUM_ADC_CONVERSIONS;x++)
        {
            m_DataBuffer[x]=0;
        }
    }
    void Init();
    void HandleInterrupt();

    //pass the path object into the ADC, it will manage moving to next path, with paths added to it while running
    ControlManager *m_pcontroller;

    static CADC *m_pSelf;
    static const int NUM_ADC_CONVERSIONS = 6;
    uint16_t m_DataBuffer[NUM_ADC_CONVERSIONS];

};




#endif /* ADC_H_ */
