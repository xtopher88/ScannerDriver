
#include "Control.h"





//main loop the pools the uart, the path list and ...
void CommandInterface::MainWileLoop()
{
    while(true)
    {
        //check for returned command
        if( m_pReturnCommand != NULL )
        {
            //if this was a blocking command send ack now
            if(m_pReturnCommand->m_DeferedAck >= 0)
            {
                SendAck();
                m_pReturnCommand->m_DeferedAck = -1;
            }
            m_CommandBuffer.AddPathToBuffer(m_pReturnCommand);
            m_ActiveCommand = false;
            m_pReturnCommand = NULL;


        }

        //if not running a command start the next command
        if(!m_ActiveCommand)
        {
            CommandBase *pCommand = m_CommandBuffer.GetNextPath();
            if(pCommand != NULL)
            {
                m_pControlManager->RunCommand(pCommand);
                m_ActiveCommand = true;
            }
            else
            {
                //if there is a pending dwell wait, send the ack when completed all commands
                if(m_bDwellWait)
                {
                    m_bDwellWait = false;
                    SendAck();
                }
            }
        }

        //check the UART for commands
        //  - if non blocking command send ack and add to buffer
        MonitorUART();

        if(m_TickCount > 5000)
        {
            m_TickCount = 0;
            m_pStatusLED->Toggle();
        }
    }
}


//return the given command after it has been completed
//need to be concerned about recursive function calls
//set a flag to know to process the next command
void CommandInterface::ReturnCommand(CommandBase *pCommand)
{
    m_pReturnCommand = pCommand;
}


//
//
//
void CommandInterface::SendAck()
{
    //TODO determine ack format and send ack here
    m_pUART->SendChar('\n');
}

void CommandInterface::MonitorUART()
{
    if(m_pUART->m_nTransferSize > 0)
    {
        //a packet has been received
        int nBufferSize = m_pUART->m_nTransferSize;
        m_pUART->m_nTransferSize = 0;
        char *pBuffer = m_pUART->m_pTransferBuffer;
        //assume at the beginning of a valid message
        COMMAND_DATA_STRUCT nCmd;
        int nIndex = 0;

        if(!GetSegment(pBuffer,nBufferSize,nIndex,nCmd))
        {
            return;
        }

        switch(nCmd.nChar)
        {
            case 'G':
            {
                ReadCode_G(pBuffer,nBufferSize,nIndex,nCmd);
                break;
            }
            case 'M':
            {
                ReadCode_M(pBuffer,nBufferSize,nIndex,nCmd);
                break;
            }
        }
    }
}


void CommandInterface::ReadCode_G(char *pBuffer,int nSize,int nIndex,COMMAND_DATA_STRUCT nData)
{
    COMMAND_DATA_STRUCT pCmdModifier[MAX_CMD_ADDITIONS];
    int nNumModifiers = 0;
    //need to read in all of the modifiers
    for(;nNumModifiers<MAX_CMD_ADDITIONS;nNumModifiers++)
    {
        if(!GetSegment(pBuffer,nSize,nIndex,pCmdModifier[nNumModifiers]))
        {
            break;
        }
    }

    float fArkI = 0.0;
    float fArkJ = 0.0;

    float fx = 0.0;
    float fy = 0.0;

    //check for all modifiers used in the g-code
    for(int i=0;i<nNumModifiers;i++)
    {
        switch(pCmdModifier[i].nChar)
        {
            case 'x':
            case 'X':
            {
                fx = pCmdModifier[i].fNum;
                break;
            }
            case 'y':
            case 'Y':
            {
                fy = pCmdModifier[i].fNum;
                break;
            }
            case 's':
            case 'S':
            {
                m_fSpeed = pCmdModifier[i].fNum;
                break;
            }
            case 'i':
            case 'I':
            {
                fArkI = pCmdModifier[i].fNum;
                break;
            }
            case 'j':
            case 'J':
            {
                fArkJ = pCmdModifier[i].fNum;
                break;
            }
        }
    }

    bool deferedAck = m_CommandBuffer.BufferFull();

    switch(nData.nNum)
    {
        case 0:
        case 1:
        {
            //G0 Xnnn Ynnn Znnn Ennn Fnnn Snnn
            //G1 Xnnn Ynnn Znnn Ennn Fnnn Snnn
            //use last or default values for items not provided
            m_CommandBuffer.AddPath({m_fX,m_fY},
                    {fx,fy},
                    m_fSpeed,
                    deferedAck);
            break;
        }
        case 2:
        { //G2 Xnnn Ynnn Innn Jnnn Ennn (Clockwise Arc)
            //use last or default values for items not provided
            m_CommandBuffer.AddArc({m_fX,m_fY},
                    {fx,fy},
                    {fArkI,fArkJ},
                    m_fSpeed,
                    true,
                    deferedAck);
            break;
        }
        case 3:
        { //G3 Xnnn Ynnn Innn Jnnn Ennn (Counter-Clockwise Arc)
            //use last or default values for items not provided
            m_CommandBuffer.AddArc({m_fX,m_fY},
                    {fx,fy},
                    {fArkI,fArkJ},
                    m_fSpeed,
                    false,
                    deferedAck);
            break;
        }
        case 4:
        {
            //G4: Dwell
            //dwell is used to ensure that each layer is done before moving vertical
            m_bDwellWait = true;
        }
    }

    m_fX = fx;
    m_fY = fy;
    //need to handle sending ack while there is room in the buffer, once the buffer is full
    //need to be able to send ack to restart sending.
    if(!deferedAck && !m_bDwellWait)
    {
        SendAck();
    }
}


void CommandInterface::ReadCode_M(char *pBuffer,int nSize,int nIndex,COMMAND_DATA_STRUCT nData)
{
    COMMAND_DATA_STRUCT pCmdModifier[MAX_CMD_ADDITIONS];
    int nNumModifiers = 0;
    //need to read in all of the modifiers
    for(;nNumModifiers<MAX_CMD_ADDITIONS;nNumModifiers++)
    {
        if(!GetSegment(pBuffer,nSize,nIndex,pCmdModifier[nNumModifiers]))
        {
            break;
        }
    }

    bool deferedAck = m_CommandBuffer.BufferFull();

    if(nData.nNum == 1)
    {
        //set p, or i for both x and y
        for(int i=0;i<nNumModifiers;i++)
        {
            switch(pCmdModifier[i].nChar)
            {
                case 'p':
                case 'P':
                {
                    m_pControlManager->SetGainP(pCmdModifier[i].fNum);
                    break;
                }
                case 'i':
                case 'I':
                {
                    m_pControlManager->SetGainI(pCmdModifier[i].fNum);
                    break;
                }
                case 'd':
                case 'D':
                {
                    m_pControlManager->SetGainD(pCmdModifier[i].fNum);
                    break;
                }

            }
        }
        deferedAck = false;
    }
    else if(nData.nNum == 3)
    {
        //M3 sets the laser velocity intensity
        for(int i=0;i<nNumModifiers;i++)
        {
            switch(pCmdModifier[i].nChar)
            {
            case 's':
            case 'S':
            {
                m_CommandBuffer.AddLaserPower(pCmdModifier[i].fNum, deferedAck);
                break;
            }
            }
        }
    }
    else if(nData.nNum == 4)
    {
        //M4 turns on the laser
        m_CommandBuffer.AddLaserOnOff(true, deferedAck);
    }
    else if(nData.nNum == 5)
    {
        //M5 turns off the laser
        m_CommandBuffer.AddLaserOnOff(false, deferedAck);
    }
    else if(nData.nNum == 10)
    {
        //M10 get current settings
        m_pUART->SendChar('X');
        m_pUART->SendInteger(m_LastData[0]);
        m_pUART->SendChar(',');
        m_pUART->SendChar('Y');
        m_pUART->SendInteger(m_LastData[1]);
        deferedAck = false;
    }
    else if(nData.nNum == 11)
    {
        for(int i=0;i<nNumModifiers;i++)
        {
            switch(pCmdModifier[i].nChar)
            {
            case 'p':
            case 'P':
            {
                m_pControlManager->SetPower(pCmdModifier[i].nNum);
                break;
            }
            }
        }
        deferedAck = false;
    }

    if(!deferedAck)
    {
        SendAck();
    }
}


bool CommandInterface::GetSegment(char *pBuffer,int nSize,int &nIndex,COMMAND_DATA_STRUCT &nData)
{
    //skip white space
    while(pBuffer[nIndex] == ' ')
    {
        nIndex++;
    }
    if(nIndex >= nSize)
        return false;
    nData.nChar = pBuffer[nIndex];
    if((nData.nChar>'A') && (nData.nChar<'Z') || (nData.nChar>'a') && (nData.nChar<'z'))
    {
        nIndex++;
        return GetNumber(pBuffer, nSize, nIndex, nData);
    }
    return false;

}


bool CommandInterface::GetNumber(char *pBuffer,int nSize,int &nIndex,COMMAND_DATA_STRUCT &nData)
{
    nData.nNum = 0;
    nData.fNum = 0.0;
    int nNegate = 1;
    // if the first character is not a number return false
    for(;nIndex<nSize;nIndex++)
    {
        char nChar = pBuffer[nIndex];
        if(nChar=='-')
        {
            nNegate = -1;
        }
        if(nChar=='.')
        {
            nIndex++;
            break;
        }
        if((nChar>='0') && (nChar<='9'))
        {
            nData.nNum = nData.nNum * 10;
            nData.nNum += nChar - '0';
        }
        if((nChar==' ') || (nChar=='/n'))
        {
            nData.nNum*=nNegate;
            nData.fNum = nData.nNum;
            return true;
        }
    }
    int nFract = 0;
    int nDivisor = 1;
    for(;nIndex<nSize;nIndex++)
    {
        char nChar = pBuffer[nIndex];
        if((nChar>='0') && (nChar<='9'))
        {
            nFract = nFract * 10;
            nDivisor = nDivisor * 10;
            nFract += nChar - '0';
        }
        else
        {
            break;
        }
    }
    nData.nNum = nData.nNum * nNegate;
    nData.fNum = nData.nNum + (float)(nNegate*nFract)/nDivisor;
    return true;
}








void ControlManager::RunCommand(CommandBase *pCommand)
{
    if(pCommand == NULL)
        return;

    CommandBase::command_type nType = pCommand->m_CommandType;
    //if the command is a path, update the path to the new command,
    switch(nType)
    {
        case CommandBase::POINT_PATH:
        case CommandBase::LINE_PATH:
        case CommandBase::ARC_PATH:
        {
            //send this command to laser path following control
            if(static_cast<CPathBase*>(pCommand)->RunMove(m_f_dt,
                    m_Position,
                    m_Velocity,
                    m_Acceleration))
            {
                m_pCommandInterface->ReturnCommand(pCommand);
            }
            else
            {
                m_pActiveCommand = pCommand;
            }
            break;
        }
        case CommandBase::LASER_POWER:
        {
            //TODO: set the global laser power setting, based on the speed of the movement this will need to be changed
            m_pLaserPower->SetDAC(static_cast<CLaserPower*>(pCommand)->m_bLaserPower);
            m_pCommandInterface->ReturnCommand(pCommand);
            break;
        }
        case CommandBase::LASER_ON_OFF:
        {
            if(static_cast<CLaserOnOff*>(pCommand)->m_bLaserOn)
            {
                m_pLaserOnOff->SetHigh();
            }
            else
            {
                m_pLaserOnOff->SetLow();
            }
            m_pCommandInterface->ReturnCommand(pCommand);
            break;
        }
        default:
        {
            m_pCommandInterface->ReturnCommand(pCommand);
        }
    }
}


//this complete can be called by any of the running controls, only one will be active at a time
void ControlManager::CommandComplete()
{
    //the running command has been completed, return the active command to the Command interface
    if(m_pActiveCommand != NULL)
    {
        m_pCommandInterface->ReturnCommand(m_pActiveCommand);
        m_pActiveCommand = NULL;
    }
}


void ControlManager::ADC_Data_Interrupt(unsigned int nXpos,
        unsigned int nYpos,
        unsigned int nXcurrent,
        unsigned int nYcurrent)
{
    //the ADC interrupt is only used to run the position control loop,
    //floating points are not used in this interrupt
    //update the tick counter
    m_pCommandInterface->m_TickCount++;

    //update the current set values
    m_pCommandInterface->m_LastData[0]=nXcurrent;
    m_pCommandInterface->m_LastData[1]=nYcurrent;

    //run the angle feedback loop for the mirror positioning
    //todo position error PI, velocity error D, and acceleration feed forward
    int  control_signal1 = m_Xcontrol.RunPIControl(m_Position.x, nXpos);
    m_pPWMControl->SetChannel1(PWM_Table::GetChannelA(control_signal1));
    m_pPWMControl->SetChannel2(PWM_Table::GetChannelB(control_signal1));
    int control_signal2 = m_Ycontrol.RunPIControl(m_Position.y, nYpos);
    m_pPWMControl->SetChannel3(PWM_Table::GetChannelB(control_signal2));
    m_pPWMControl->SetChannel4(PWM_Table::GetChannelA(control_signal2));
}


void ControlManager::RunPositionUpdate()
{
    //if there is a path to follow then update the current position
    if( (m_pActiveCommand != NULL)) //m_bActivePath &&
    {
        bool bComplete = static_cast<CPathBase*>(m_pActiveCommand)->RunMove(m_f_dt, m_Position, m_Velocity, m_Acceleration);
        if(bComplete)
        {
            CommandComplete();
        }
    }
}


