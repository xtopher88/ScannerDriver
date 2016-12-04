/// This file contains the base classes which others are built on

#include "Include.h"

#ifndef _BASECLASSES_H
#define _BASECLASSES_H

/// \class BaseDriver
/// \breif a base class for hardware divers
class CBaseDriver
{
protected:
    CBaseDriver *m_pNext;
    void ReturnAction(int nAction);
public:
    virtual void RunPoling()
    {
    }
    CBaseDriver(CBaseDriver *pNext = NULL)
    {
        SetNext( pNext );
    }
    void SetNext( CBaseDriver *pNext )
    {
        m_pNext = pNext;
    }
    virtual void Init()
    { 
    }
    virtual void Interupt()
    {
    }
    //return from a calling class
    virtual void ReturnCall(int nInfo)
    {
    }
};

class CComInterface
{
public:
    CComInterface()
    {
    }
//protected:    
//Transmits single data byte
    virtual bool SendChar( char cTransmit ){ return false; };
//Returns the most recent received data byte
    virtual bool RecieveChar( char &cReceive ){ return false; };
    //check to see if transmit is ready for the next byte
    virtual bool ReadyToSend(){ return false; };
    //check to see if there is a byte waiting to be read
    virtual bool RecieveReady(){ return false; };
};

//the com buffer is to seporate the handling of the data from the sending and receiving of the data
class CComBufferInterface
{
public:
    virtual bool GetNextTx(char &cNext){ return false; };
    virtual bool SaveRx(char cRecieved){ return false; };
};

//the com driver is a combination of the base driver information and the communication interface
//this is provided as a base class for interfacing with the varius communications drivers
class CComDriver: public CComInterface, public CBaseDriver
{
protected:
    CComBufferInterface * m_pData;
    bool m_bTxActive;
public:
    CComDriver( CComBufferInterface * pData,
                CBaseDriver *pNext = NULL
                ):CComInterface(),
                CBaseDriver(pNext),
                m_pData(pData),
                m_bTxActive(false)
    {
    }
    //a function to call to start the transmit if it was not running
    void StartTx();
    virtual void SetInterupt( bool bEnable = true ){};
    virtual void Interupt();
    enum RETURN_ACTIONS
    {
        ERROR,
        TRANSMIT_BUFFER_EMPTY,
        RECEIVE_BUFFER_EMPTY,
        BYTE_TX,
        BYTE_RX
    };
};


//a tx and rx buffer wich just adds to teh rx buffer and reads from the tx buffer
class CSyncComBuffer: public CComBufferInterface
{
public:
    CBuffer<char,64> m_TxBuffer;
    CBuffer<char,64> m_RxBuffer;
    
    CSyncComBuffer():CComBufferInterface(),m_TxBuffer(),m_RxBuffer()
    {
    }
    virtual bool GetNextTx(char &cNext);
    virtual bool SaveRx(char cRecieved);
};

#endif //_PERIPHERAL_H
