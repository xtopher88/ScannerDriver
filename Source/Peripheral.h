/// Peripheral
// this file contains the implementation of the IO layer for the STM32 chip
// This layer is meant to separate the hardware specifics from the application 

#include "stm32f4xx_conf.h" 
#include "stm32f4xx_rcc.h"
#include "Include.h"
#include "BaseClasses.h"

#ifndef _PERIPHERAL_H
#define _PERIPHERAL_H


//*****************************************************************
//          CUARTDriver 
//*****************************************************************
class CUARTDriver: public CComDriver
{
protected:
    USART_TypeDef * m_UARTx;
    //Send and receive functions
    virtual bool SendChar( char cTx );
    virtual bool RecieveChar( char &cRx );
    //Set the transmit interupt 
    virtual void SetInterupt( bool bEnable = true );
    //check to see if transmit is ready for the next byte
    virtual bool ReadyToSend();
    //check to see if there is a byte waiting to be read
    virtual bool RecieveReady();
public:
    CUARTDriver(  CComBufferInterface * pData, USART_TypeDef * UARTx ):CComDriver(pData),m_UARTx(UARTx)
    {
    }
};

//*****************************************************************
//          CUART2Com 
//*****************************************************************
class CUART2Com:public CUARTDriver
{
protected:
    unsigned int m_nBaudRate;
public:
    CUART2Com( CComBufferInterface * pData, unsigned int nBaud = 9600
                ):CUARTDriver( pData, USART2 ),m_nBaudRate(nBaud)
    {
    }
    virtual void Init();
};


//*****************************************************************
//          CI2CDriver 
//*****************************************************************

/*
class CI2CDriver: public CComDriver
{
protected:
    I2C_TypeDef * m_I2Cx;
    virtual bool SendChar( char cTx );
    virtual bool RecieveChar( char &cRx );
    virtual bool ComInterupt( bool bTx, char cTx, bool & bRx, char &cRx);
public:
    CI2CDriver(  I2C_TypeDef * pData, I2C_TypeDef * I2Cx ):CComDriver(pData),m_I2Cx(I2Cx)
    {
    }
    //a function to call to start the transmit if it was not running
    void StartTx()
    {
        RunInterupt();
    }
};


class CI2CCom1: public CComBase
 {
 public:
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    virtual void InitDevice();
    virtual void RunInterupt();
    virtual void SetInterupt()
    {
        //USART_ITConfig(USART2, USART_IT_TC, ENABLE);
    }
    virtual void SendChar(char cVal)
    {
        //USART_SendData( USART2, (uint8_t) cVal);
       // Send I2C1 START condition 
  
    }
    virtual char RecieveChar()
    {
      I2C_GenerateSTART(I2C1, ENABLE);

  // Test on I2C1 EV5 and clear it 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  // Send EEPROM slave Address for write 
  I2C_Send7bitAddress(I2C1, 0x0E, I2C_Direction_Receiver);//I2C_Direction_Transmitter);

  // Test on I2C1 EV6 and clear it 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  // Send I2C1 EEPROM internal address 
  I2C_SendData(I2C1, 0x0F);

  // Test on I2C1 EV8 and clear it 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  // Send I2C1 EEPROM data 
  char nByte = I2C_ReceiveData( I2C1 );//I2C_SendData(I2C1, 0x05);

  // Test on I2C1 EV8 and clear it 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));// I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  // Send I2C1 STOP Condition 
  I2C_GenerateSTOP(I2C1, ENABLE);
        return nByte;
    }
public:
    CI2CCom1():CComBase()
    {
       // m_nBaudRate = nBaud;
       // m_pTxBuffer = &m_TxBuffer;
       // m_pRxBuffer = &m_RxBuffer;
    }
};
*/



#endif //_PERIPHERAL_H
