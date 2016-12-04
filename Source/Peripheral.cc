
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_tim.h"
#include "Peripheral.h"
#include "misc.h"


//*****************************************************************
//          CUARTDriver 
//*****************************************************************
bool CUARTDriver::SendChar( char cTx )
{
    USART_SendData( m_UARTx, (uint8_t) cTx);
    return true;
}
bool CUARTDriver::RecieveChar( char &cRx )
{
    cRx = USART_ReceiveData(m_UARTx);
    return true;
}

//Set the transmit interupt 
void CUARTDriver::SetInterupt( bool bEnable )
{
    if( bEnable )
    {
        USART_ITConfig(m_UARTx, USART_IT_TC, ENABLE);
    }
    else
    {
        USART_ITConfig(m_UARTx, USART_IT_TC, DISABLE);
    }
}
//check to see if transmit is ready for the next byte
bool CUARTDriver::ReadyToSend()
{
    return USART_GetITStatus(m_UARTx, USART_IT_TC) == SET;
}
//check to see if there is a byte waiting to be read
bool CUARTDriver::RecieveReady()
{
    return USART_GetITStatus(m_UARTx, USART_IT_RXNE) == SET;
}
    
//*****************************************************************
//          CUART2Com 
//*****************************************************************
void CUART2Com::Init()
{
    // Enable USART clock 
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USARTx, ENABLE) //for USART1 and USART6 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //for USART2, USART3, UART4 or UART5.
    // Enable GPIO clock 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
    // USART2 GPIO configuration 
    // Connect USART pins to AF7
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
    // Configure USART Tx and Rx as alternate function push-pull
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    // Enable the USART OverSampling by 8 
    USART_OverSampling8Cmd(USART2, ENABLE);
    // USART2 configuration
    USART_InitTypeDef USART_InitStructure; 
    USART_InitStructure.USART_BaudRate = m_nBaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART2, ENABLE);
    // Enable the USARTx Interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //enable the recieve interupt
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}







