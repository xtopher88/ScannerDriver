/*
 * GPIO.h
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f4xx_conf.h"
#include "stm32f4xx_rcc.h"

//*****************************************************************
//          CGPIO
//*****************************************************************
//the GPIO class saves the port and pin information, and provides the on and off command set
//the child class must initialize the port
class CGPIO
{
protected:
    GPIO_TypeDef * m_GPIOx;
    uint16_t m_GPIO_Pin_x;
    uint32_t m_RCC_AHB1Periph;
public:
    CGPIO( uint32_t RCC_AHB1Periph,
           GPIO_TypeDef * GPIOx,
           uint16_t GPIO_Pin_x ):m_GPIOx(GPIOx),
                                m_GPIO_Pin_x(GPIO_Pin_x),
                                m_RCC_AHB1Periph(RCC_AHB1Periph)
    {
    }
    virtual void SetHigh();
    virtual void SetLow();
    virtual void Toggle();
    virtual bool GetState();
};

//*****************************************************************
//          CGPIO_PP
//*****************************************************************
//GPIO pin set up as a push pull output
class CGPIO_PP: public CGPIO
{
public:
    CGPIO_PP( uint32_t RCC_AHB1Periph,
              GPIO_TypeDef * GPIOx,
              uint16_t GPIO_Pin_x ):CGPIO(RCC_AHB1Periph, GPIOx, GPIO_Pin_x)
    {
    }
    virtual void Init();
};

//*****************************************************************
//          CGPIO_OD
//*****************************************************************
//open drain GPIO output
class CGPIO_OD: public CGPIO
{
public:
    CGPIO_OD( uint32_t RCC_AHB1Periph,
              GPIO_TypeDef * GPIOx,
              uint16_t GPIO_Pin_x ):CGPIO(RCC_AHB1Periph, GPIOx, GPIO_Pin_x)
    {
    }
    virtual void Init();
};



#endif /* GPIO_H_ */
