/// This file contains the base classes which others are built on

#include "Peripheral.h"
#include "Include.h"

#ifndef _UARTDEVICE_H
#define _UARTDEVICE_H


/// the SPI device shall have 
/// - an interface to the chip select controller
/// - a data buffer for recieved and transmit buffers
/// - the initialization of and access to the given SPI device
//*****************************************************************
//          CSPIDriver 
//*****************************************************************
//general SPI driver functions
class CUARTDevice
{
protected:
    CharBufferBase * m_pTxBuffer;
    USART_TypeDef * m_UARTx;

    char *m_pRxBuffer;
    int m_nRxIndex;
    int m_RxBufferSize;
    //keep track if the transmit is active
    bool m_bTxActive;
    
    //Set the transmit interupt 
    void SetInteruptTC( bool bEnable = true );
    
    //check to see if transmit is ready for the next byte
    bool ReadyToSend();
    //check to see if there is a byte waiting to be read
    bool RecieveReady();
    //enable the transmit and recieve interupt and set the transmit ready bit
    void StartTransmit();
public:
    char *m_pTransferBuffer;
    int m_nTransferSize;

    CUARTDevice(CharBufferBase *pTxBuffer,
                USART_TypeDef * pUARTx,
                char *pRxBuffer,
                char *pTransferBuffer,
                int RxBufferSize):m_pTxBuffer(pTxBuffer),
                    m_UARTx(pUARTx),
                    m_bTxActive(false),
                    m_pRxBuffer(pRxBuffer),
                    m_pTransferBuffer(pTransferBuffer),
                    m_RxBufferSize(RxBufferSize)
    {
        m_nRxIndex = 0;
        m_nTransferSize = 0;
    }
    bool AddArray( char * cArray, int nSize );
    bool AddChar( char cSend );
    /// handle the transmit and receive interrupt
    void HandleInterupt();
    /// Sets up the specifics for the SPI, and pins
    virtual void Init() = 0;

    /// calling function to start a transmit receive
    bool SendData( char * cArray, int nSize );
    bool SendChar( char cTx );
    bool SendInteger(unsigned int nValue);
    bool SendInteger(int nValue);
};

/// specific initialization
class CUART3Port: public CUARTDevice
{
protected:
    unsigned int m_nBaudRate;
    //transmitt and recieve buffers
    CBuffer<char,1024> m_TxBuffer;
    static const int RX_BUFFER_SIZE = 256;
    char m_RxBuffer[RX_BUFFER_SIZE];
    int m_nRxIndex;
public:
    char m_TransferBuffer[RX_BUFFER_SIZE];
    int m_nTransferIndex;
    CUART3Port(unsigned int nBaud = 57600
                ):CUARTDevice( &m_TxBuffer,
                        USART3,
                        m_RxBuffer,
                        m_TransferBuffer,
                        RX_BUFFER_SIZE),
                   m_nBaudRate(nBaud)
    {
        m_nTransferIndex=0;
        m_nRxIndex=0;
    }
    /// Sets up the specifics for the SPI, and pins
    void Init();
};


#endif //_PERIPHERAL_H
