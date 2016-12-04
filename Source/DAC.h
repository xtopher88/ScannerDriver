/*
 * DAC.h
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */

#ifndef DAC_H_
#define DAC_H_

#include "stm32f4xx_conf.h"
#include "stm32f4xx_rcc.h"
#include "Include.h"

//*****************************************************************
//          CDAC
//*****************************************************************
class CDAC
{
protected:
public:
	CDAC()
    {
    }
    void Init();

    void SetDAC(float fValue); //0 to 3.3 floating point value
};




#endif /* DAC_H_ */
