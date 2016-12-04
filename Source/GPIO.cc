/*
 * GPIO.cc
 *
 *  Created on: Jul 19, 2014
 *      Author: kevin
 */


#include "stm32f4xx_gpio.h"
#include "GPIO.h"


//*****************************************************************
//          GPIO Devices
//*****************************************************************
void CGPIO::SetLow()
{
    GPIO_ResetBits( m_GPIOx, m_GPIO_Pin_x );
}
void CGPIO::SetHigh()
{
    GPIO_SetBits( m_GPIOx, m_GPIO_Pin_x );
}
void CGPIO::Toggle()
{
    GPIO_ToggleBits( m_GPIOx, m_GPIO_Pin_x );
}
bool CGPIO::GetState()
{
    return GPIO_ReadOutputDataBit( m_GPIOx, m_GPIO_Pin_x ) != 0;
}

//*****************************************************************
//          GPIO Push Pull
//*****************************************************************
void CGPIO_PP::Init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    //GPIOC Periph clock enable
    RCC_AHB1PeriphClockCmd(m_RCC_AHB1Periph, ENABLE);
    //Configure PC6  for the user LED
    GPIO_InitStructure.GPIO_Pin = m_GPIO_Pin_x;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    //Init the GPIO
    GPIO_Init(m_GPIOx, &GPIO_InitStructure);\
}

//*****************************************************************
//          GPIO Open Drain
//*****************************************************************
void CGPIO_OD::Init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    //GPIOC Periph clock enable
    RCC_AHB1PeriphClockCmd(m_RCC_AHB1Periph, ENABLE);
    //Configure PC6  for the user LED
    GPIO_InitStructure.GPIO_Pin = m_GPIO_Pin_x;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    //Init the GPIO
    GPIO_Init(m_GPIOx, &GPIO_InitStructure);\
}
