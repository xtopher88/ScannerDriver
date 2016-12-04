
#ifndef SOURCE_CONTROL_H_
#define SOURCE_CONTROL_H_


#include "Path.h"
#include "Include.h"
#include "Utility.h"
#include "UARTDevice.h"
#include "GPIO.h"
#include "PWM.h"
#include "DAC.h"



// serial interface
//  - receive commands from the controlling PC
//  - send responses and data to the controling PC
// G-Code
//  - Immediately handled g-code
//    -requests for data and emergency stop are handled when they are received
//  - Buffered g-code
//    -commands that can be buffered
//  - Must complete before ack g-code
//    -these command are added to the end of the buffer, once this command is complete the ack is sent
//    -have a wait for complete commands to wait until the buffer is empty for z axis movement

//control block
// receives commands
// handles sending ack for commands
// command buffer, complete tracking,
//  - holds the list of commands and the type of these commands
//    - send command to processor (path,laser control,timer,z motor)
//    - waits for complete from the last command to move on
// path following control loop
//  -calculate the desired position based on time
//  -send complete to the calling command
//  -waits for the next command to be sent
// laser power control
// z motor control

//TBD SD Card reading
//TBD reprap lcd screen control

#ifndef MAX_CMD_ADDITIONS
#define MAX_CMD_ADDITIONS 5
#endif

#ifndef LINE_ARRAY_SIZE
#define LINE_ARRAY_SIZE 20
#endif

#ifndef ARC_ARRAY_SIZE
#define ARC_ARRAY_SIZE 20
#endif

#ifndef POINT_ARRAY_SIZE
#define POINT_ARRAY_SIZE 20
#endif

#ifndef LASER_CONTROL_SIZE
#define LASER_CONTROL_SIZE 10
#endif

struct COMMAND_DATA_STRUCT
{
    char nChar;
    int nNum;
    float fNum;
    void Clear()
    {
        nChar = 0;
        nNum = 0;
        fNum =0.0;
    }
};


class ControlInterface
{
public:
    virtual void RunCommand(CommandBase *pCommand) = 0;

    virtual void SetGainP(float fGain) = 0;
    virtual void SetGainI(float fGain) = 0;
    virtual void SetGainD(float fGain) = 0;
    virtual void SetPower(unsigned int MaxControl) = 0;
};

//
//
class CommandInterface
{
    //buffer of commands handlers is maintained here
    //  the class is filled out here and sent to the control manager
    //  need an ID to track the commands to manage the ack for the unbuffered command
    //

    ControlInterface *m_pControlManager;
    CUARTDevice *m_pUART;
    CGPIO *m_pStatusLED;

    //path list
    CommandList m_CommandBuffer;
    CPathLine m_LineArray[LINE_ARRAY_SIZE];
    CPathArc m_ArcArray[ARC_ARRAY_SIZE];
    CPathPoint m_PointList[POINT_ARRAY_SIZE];
    CLaserOnOff m_LaserOnOffList[LASER_CONTROL_SIZE];
    CLaserPower m_LaserPowerList[LASER_CONTROL_SIZE];
    CommandBase *m_pReturnCommand;
    bool m_ActiveCommand;
    bool m_bDwellWait;

    //tracking of last commanded position and speed in case it was not part of the current command
    float m_fX;
    float m_fY;
    float m_fSpeed;
public:
    int m_TickCount;
    int m_LastData[2];

    CommandInterface(ControlInterface *pControlManager,
            CUARTDevice *pUART,
            CGPIO *pStatusLED):m_pControlManager(pControlManager),
            m_pUART(pUART),
            m_pStatusLED(pStatusLED),
            m_CommandBuffer(),
            m_pReturnCommand(NULL),
            m_ActiveCommand(false),
            m_bDwellWait(false),
            m_fX(0.0),
            m_fY(0.0),
            m_fSpeed(0.0),
            m_TickCount(0)
    {
        for( int x = 0; x < LINE_ARRAY_SIZE; x++ )
        {
            m_CommandBuffer.AddPathToBuffer(&m_LineArray[x]);
        }
        for( int x = 0; x < ARC_ARRAY_SIZE; x++ )
        {
            m_CommandBuffer.AddPathToBuffer(&m_ArcArray[x]);
        }
        for( int x = 0; x < POINT_ARRAY_SIZE; x++ )
        {
            m_CommandBuffer.AddPathToBuffer(&m_PointList[x]);
        }
        for( int x = 0; x < LASER_CONTROL_SIZE; x++ )
        {
            m_CommandBuffer.AddPathToBuffer(&m_LaserOnOffList[x]);
            m_CommandBuffer.AddPathToBuffer(&m_LaserPowerList[x]);
        }
    }

    //main loop the pools the uart, the path list and ...
    void MainWileLoop();

    //return the given command after it has been completed
    //need to be concerned about recursive function calls
    //set a flag to know to process the next command
    void ReturnCommand(CommandBase *pCommand);

    void SendAck();

    //read g and m code

    //commands to create paths and add them to the buffers

    void MonitorUART();
    void ReadCode_G(char *pBuffer,int nSize,int nIndex,COMMAND_DATA_STRUCT nData);
    void ReadCode_M(char *pBuffer,int nSize,int nIndex,COMMAND_DATA_STRUCT nData);
    bool GetSegment(char *pBuffer,int nSize,int &nIndex,COMMAND_DATA_STRUCT &nData);
    bool GetNumber(char *pBuffer,int nSize,int &nIndex,COMMAND_DATA_STRUCT &nData);

};


class ControlManager: public ControlInterface
{
protected:
    //the PID Control algorithm for running the mirrors lives here
    CPIControlInt m_Xcontrol;
    CPIControlInt m_Ycontrol;
    CPWM *m_pPWMControl;
    CGPIO *m_pLaserOnOff;
    CDAC *m_pLaserPower;
    float m_f_dt;
    FloatVector m_Position;
    FloatVector m_Velocity;
    FloatVector m_Acceleration;
public:
    CommandBase *m_pActiveCommand;
    bool m_bActivePath;
    CommandInterface *m_pCommandInterface;
    ControlManager(CommandInterface *pCommandInterface,
            CPWM *pPWMControl,
            CGPIO *pLaserOnOff,
            CDAC *pLaserPower,
            float f_dt = 1.0/25000.0
            ):ControlInterface(),
                    m_pCommandInterface(pCommandInterface),
                    //m_Xcontrol(f_dt,0.001,0.04,0.02,150),
                    //m_Ycontrol(f_dt,0.001,0.04,0.02,150),
                    m_Xcontrol(f_dt,0.25,0.00001,0.1,150),
                    m_Ycontrol(f_dt,0.25,0.00001,0.1,150),
                    m_pPWMControl(pPWMControl),
                    m_pLaserOnOff(pLaserOnOff),
                    m_pLaserPower(pLaserPower),
                    m_f_dt(f_dt),
                    m_Position(2000.0, 2000.0),
                    m_pActiveCommand(NULL),
                    m_bActivePath(false)
    {

    }

    void RunCommand(CommandBase *pCommand);
    //this complete can be called by any of the running controls, only one will be active at a time
    void CommandComplete();

    // Set new ADC Values called from the ADC data ready interrupt
    void ADC_Data_Interrupt(unsigned int nXpos,
            unsigned int nYpos,
            unsigned int nXcurrent,
            unsigned int nYcurrent);

    //path following update
    void RunPositionUpdate();

    //Laser Position control interface
    //float f_dt = 1.0/10000.0
    void SetGainP(float fGain)
    {
        m_Xcontrol.SetkP(fGain);
        m_Ycontrol.SetkP(fGain);
    }
    void SetGainI(float fGain)
    {
        m_Xcontrol.SetkI(fGain);
        m_Ycontrol.SetkI(fGain);
    }
    void SetGainD(float fGain)
    {
        m_Xcontrol.SetkD(fGain);
        m_Ycontrol.SetkD(fGain);
    }
    void SetPower(unsigned int MaxControl)
    {
        m_Xcontrol.SetMaxControl(MaxControl);
        m_Ycontrol.SetMaxControl(MaxControl);
    }
};





#endif
