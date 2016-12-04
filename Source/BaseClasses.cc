
#include "BaseClasses.h"



void CBaseDriver::ReturnAction(int nAction)
{
    if( m_pNext != NULL)
        m_pNext->ReturnCall( nAction );
}


void CComDriver::StartTx()
{
    SetInterupt( true );
}
void CComDriver::Interupt()
{
    //check to see if there is a recieved byte
    if( RecieveReady() )
    {
        char cRxData = 0;
        if( RecieveChar( cRxData ) ) //todo handle recieve failures
        {
            m_pData->SaveRx( cRxData );
            ReturnAction( BYTE_RX );
        }
    }
    //check to see if the COM device is ready for another transmit byte
    if( ReadyToSend() )
    {
        //check for and get the next byte to send
        char cTxData = 0;
        if( m_pData->GetNextTx( cTxData ) )
        {
            SendChar( cTxData ); //TODO handle transmit failure
            m_bTxActive = true;
        }
        else
        {
            //disable the transmit buffer
            SetInterupt( false );
            if( m_bTxActive )
            {
                m_bTxActive = false;
                ReturnAction( TRANSMIT_BUFFER_EMPTY );
            }
        }
    }
}




bool CSyncComBuffer::GetNextTx(char &cNext)
{
    return m_TxBuffer.Get( cNext );
}
bool CSyncComBuffer::SaveRx(char cRecieved)
{
    return m_RxBuffer.Add( cRecieved );
}

