/*
 * Path.h
 *
 *  Created on: Jan 14, 2015
 *      Author: kchristopher
 */

#ifndef SOURCE_PATH_H_
#define SOURCE_PATH_H_


#include "Include.h"
#include "arm_math.h"
#include "Path.h"
#include "Utility.h"

class CommandBase
{
public:
    enum command_type
    {
        INVALID_COMMAND,
        POINT_PATH,
        LINE_PATH,
        ARC_PATH,
        LASER_POWER,
        LASER_ON_OFF
    };
    CommandBase()
    {
        m_pNext = NULL;
        m_CommandType = INVALID_COMMAND;
        m_DeferedAck = -1;
    }
    CommandBase *m_pNext;
    command_type m_CommandType;
    int m_DeferedAck;
};

class CLaserOnOff:public CommandBase
{
public:
    bool m_bLaserOn;
    CLaserOnOff():CommandBase(),m_bLaserOn(false)
    {
        m_CommandType = LASER_ON_OFF;
    }
};

class CLaserPower:public CommandBase
{
public:
    float m_bLaserPower;
    CLaserPower():CommandBase(),m_bLaserPower(0.0)
    {
        m_CommandType = LASER_POWER;
    }
};

class CPathBase:public CommandBase
{
public:
    CPathBase():CommandBase()
    {
    }
    /// returns true if complete
    virtual bool RunMove(float dT,  FloatVector &position,  FloatVector &velocity,  FloatVector &acceleration) = 0;
};


class CPathPoint:public CPathBase
{
public:
    FloatVector m_nPos;
    float m_nTime;
    float m_nFinishTime;
    CPathPoint():CPathBase(),m_nTime(0.0),m_nFinishTime(0.0)
    {
        m_CommandType = POINT_PATH;
    }
    void SetUpMove(FloatVector nTargetPos, float fDelay);
    bool RunMove(float dT,  FloatVector &position,  FloatVector &velocity,  FloatVector &acceleration);
};


class CPathLine:public CPathBase
{
public:
    CPathLine():CPathBase()
    {
        m_CommandType = LINE_PATH;
        reset();
    }

    void reset()
    {
        m_nTargetPos = {0.0, 0.0};
        m_nTargetVelocity = {0.0, 0.0};
        m_nAcceleration = {0.0, 0.0};

        m_nTime = 0.0;
        m_tAccel = 0.0;
        m_tConstVelocity = 0.0;
        m_tFinish = 0.0;
        m_Acceleration = 500000;
    }

    float m_nTime;
    FloatVector m_nTargetPos;
    FloatVector m_nTargetVelocity;
    FloatVector m_nAcceleration;

    //FloatVector m_nPosition;
    //FloatVector m_nVelocity;


    float m_tAccel;
    float m_tConstVelocity;
    float m_tFinish;
    float m_Acceleration;

    enum MOVE_STEP
    {
        START_MOVE,
        ACCELERATION,
        CONSTANT_VELOC,
        DECELERATION
    };

    MOVE_STEP m_nStep;

    void SetUpMove(FloatVector nInitPos, FloatVector nTargetPos,float fRate);
    bool RunMove(float dT,  FloatVector &position,  FloatVector &velocity,  FloatVector &acceleration);
};


class CPathArc:public CPathBase
{
public:
    CPathArc():CPathBase(),
        m_tAccel(0.0),
        m_tConstVelocity(0.0),
        m_tFinish(0.0),
        m_nTime(0.0),
        m_AngularRate(0.0),
        m_AngularAcceleration(0.0),
        m_radius(0.0),
        m_angle(0.0),
        m_finish_angle(0.0),
        m_bClockwise(true)
    {
        m_CommandType = ARC_PATH;
        m_Acceleration = 500000;
    }
    FloatVector m_nFinishPosition;
    FloatVector m_nCenterPoint;
    float m_nTime;
    float m_AngularRate;
    float m_TargetAngularRate;
    float m_AngularAcceleration;
    float m_radius;
    float m_angle;
    float m_finish_angle;
    bool m_bClockwise;


    float m_tAccel;
    float m_tConstVelocity;
    float m_tFinish;
    float m_Acceleration;


    enum MOVE_STEP
    {
        START_MOVE,
        ACCELERATION,
        CONSTANT_VELOC,
        DECELERATION
    };

    MOVE_STEP m_nStep;

    static constexpr float PI = 3.141592653589793;

    void SetUpMove(FloatVector nInitPos, FloatVector nTargetPos, FloatVector nCenterPoint, float fRate, bool bClockwise);
    bool RunMove(float dT,  FloatVector &position,  FloatVector &velocity,  FloatVector &acceleration);
};


//TODO add intensity and control of the DAC laser current control, based on the movement rate adjust the laser
class CommandList
{
public:
    CommandList():m_pStart(NULL),
        m_pFinish(NULL),
        m_pPointList(NULL),
        m_pLineList(NULL),
        m_pArcList(NULL),
        m_pLaserOnOffList(NULL),
        m_pLaserPowerList(NULL),
        m_nPointNum(0),
        m_nLineNum(0),
        m_nArcNum(0)
    {
    }
    CommandBase *m_pStart;
    CommandBase *m_pFinish;
    //empty path lists
    CPathPoint *m_pPointList;
    CPathLine *m_pLineList;
    CPathArc *m_pArcList;
    CLaserOnOff *m_pLaserOnOffList;
    CLaserPower *m_pLaserPowerList;
    //available path tracker
    int m_nPointNum;
    int m_nLineNum;
    int m_nArcNum;
    int m_nLaserOnOffNum;
    int m_nLaserPowerNum;

    bool BufferFull()
    {
        if((m_nPointNum > 1) &&
           (m_nLineNum > 1)&&
           (m_nArcNum > 1)&&
           (m_nLaserOnOffNum > 1)&&
           (m_nLaserPowerNum > 1))
        {
            return false;
        }
        return true;
    }


    //list of path commands
    CommandBase * GetNextPath();
    void AddToEnd(CommandBase *pPath);

    void AddPathToBuffer(CPathPoint *pPointPath);
    void AddPathToBuffer(CPathLine *pLinePath);
    void AddPathToBuffer(CPathArc *pArcPath);
    void AddPathToBuffer(CLaserOnOff *pLaserControl);
    void AddPathToBuffer(CLaserPower *pLaserControl);
    void AddPathToBuffer(CommandBase *pBaseCommand);
    //return false if not added
    bool AddArc(FloatVector nInitPos,
            FloatVector nTargetPos,
            FloatVector nCenterPoint,
            float fRate,
            bool bClockwise,
            bool deferedAck = false);
    bool AddPath(FloatVector nInitPos,
            FloatVector nTargetPos,
            float fRate,
            bool deferedAck = false);
    bool AddPoint(FloatVector nInitPos,
            float fDelay,
            bool deferedAck = false);
    bool AddLaserPower(float fPower,
            bool deferedAck = false);
    bool AddLaserOnOff(bool bLaserOn,
            bool deferedAck = false);
};


#endif /* SOURCE_PATH_H_ */
