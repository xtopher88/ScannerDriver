
#include "BaseClasses.h"
#include "UARTDevice.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"





//    CharBufferBase * m_pRxBuffer;
//    CharBufferBase * m_pTxBuffer;
//    SPI_TypeDef * m_SPIx;
//Set the transmit interupt
void CUARTDevice::SetInteruptTC( bool bEnable )
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
bool CUARTDevice::ReadyToSend()
{
    return USART_GetITStatus(m_UARTx, USART_IT_TC) == SET;
}
//check to see if there is a byte waiting to be read
bool CUARTDevice::RecieveReady()
{
    return USART_GetITStatus(m_UARTx, USART_IT_RXNE) == SET;
}

//Interupt Handler,
void CUARTDevice::HandleInterupt()
{
    //check to see if the COM device is ready for another transmit byte
    if( ReadyToSend() )
    {
        //check for and get the next byte to send
        char cTxData = 0;
        if( m_pTxBuffer->Get( cTxData ) )
        {
            USART_SendData( m_UARTx, (uint8_t) cTxData);
        }
        else
        {
            //disable the transmit interupt
            SetInteruptTC( false );
            m_bTxActive = false;
        }
    }

    //check to see if there is a recieved byte
    if( RecieveReady() )
    {
        char cData = USART_ReceiveData(m_UARTx);

        //protection against exceeding the buffer size
        if(m_nRxIndex >= m_RxBufferSize)
        {
            if(cData == 10)
            {
                m_nRxIndex = 0;
            }
            return;
        }

        m_pRxBuffer[m_nRxIndex] = cData;
        m_nRxIndex++;

        if(cData == 10)
        {
            //copy this to a new buffer and set the signal ready flag
            //the main loop needs to look at the transfer index to read input messages
            for(unsigned int i = 0; i< m_nRxIndex; i++)
            {
                m_pTransferBuffer[i]=m_pRxBuffer[i];
            }
            m_nTransferSize = m_nRxIndex;
            m_nRxIndex = 0;
        }
    }
}


//enable the transmit and recieve interupt and set the transmit ready bit
void CUARTDevice::StartTransmit()
{
    if( !m_bTxActive )
    {
        //enable the transmit active bit
        m_bTxActive = true;
        //enable the transmit interupt, this will start the transmit by activating this interupt
        SetInteruptTC( true );
    }
}

bool CUARTDevice::AddArray( char * cArray, int nSize )
{
    for( int i = 0; i < nSize; i++ )
    {
        if( !m_pTxBuffer->Add(cArray[i]) )
            return false;
    }
    StartTransmit();
    return true;
}

bool CUARTDevice::AddChar( char cSend )
{
    if( !m_pTxBuffer->Add( cSend ) )
        return false;
    StartTransmit();
    return true;
}

/// calling function to start a transmit receive
bool CUARTDevice::SendData( char * cArray, int nSize )
{
    if( AddArray( cArray, nSize ) )
    {
       // SetNext( pNext );
        return true;
    }
    return false;
}

bool CUARTDevice::SendChar( char cTx )
{
    return AddChar( cTx );
}

bool CUARTDevice::SendInteger(unsigned int nValue)
{
    char pbuffer[10];
    unsigned int nTemp;
    unsigned int i = 1;
    for(i = 1; i<=10; i++)
    {
        nTemp = nValue / 10;
        pbuffer[10-i] = nValue-nTemp*10 + '0';
        nValue = nTemp;
        if(nValue == 0)
        {
            break;
        }
    }
    return SendData(&pbuffer[10-i],i);
}

bool CUARTDevice::SendInteger(int nValue)
{
    bool negative = false;
    if(nValue < 0)
    {
        nValue = -nValue;
        negative = true;
    }
    char pbuffer[10];
    unsigned int nTemp;
    unsigned int i = 1;
    for(i = 1; i<=10; i++)
    {
        nTemp = nValue / 10;
        pbuffer[10-i] = nValue-nTemp*10 + '0';
        nValue = nTemp;
        if(nValue == 0)
        {
            break;
        }
    }
    if(negative)
    {
        i++;
        pbuffer[10-i] = '-';
    }
    return SendData(&pbuffer[10-i],i);
}

void CUART3Port::Init()
{
	// TX PB10
	// RX PB11
    // Enable USART clock 
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USARTx, ENABLE) //for USART1 and USART6 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //for USART2, USART3, UART4 or UART5.
    // Enable GPIO clock 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
    // USART3 GPIO configuration
    // Connect USART pins to AF7
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    // Configure USART Tx and Rx as alternate function push-pull
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // Enable the USART OverSampling by 8 
    USART_OverSampling8Cmd(USART3, ENABLE);
    // USART2 configuration
    USART_InitTypeDef USART_InitStructure; 
    USART_InitStructure.USART_BaudRate = m_nBaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART3, ENABLE);
    // Enable the USARTx Interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //enable the recieve interupt
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

