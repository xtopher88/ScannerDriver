/*
 * Path.cc
 *
 *  Created on: Jan 14, 2015
 *      Author: kchristopher
 */

#include "Path.h"

#include <math.h>
#include "misc.h"




void CPathPoint::SetUpMove(FloatVector nTargetPos, float fDelay)
{
    m_nTime = 0.0;
    m_nFinishTime = fDelay;
    m_nPos = nTargetPos;
}

bool CPathPoint::RunMove(float dT,  FloatVector &position,  FloatVector &velocity,  FloatVector &acceleration)
{
    position = m_nPos;
    velocity.x = 0.0;
    velocity.y = 0.0;
    acceleration.x = 0.0;
    acceleration.y = 0.0;
    if(m_nTime >= m_nFinishTime)
        return true;
    m_nTime += dT;
    return false;
}

void CPathLine::SetUpMove(FloatVector nInitPos, FloatVector nTargetPos,float fRate)
{
    m_nTime = 0.0;
    FloatVector delta_pos =  nTargetPos - nInitPos;
    float distance = vsqrtf(delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y);
    if(distance<=0.0)
    {
        m_nTargetVelocity.x = 0.0;
        m_nTargetVelocity.y = 0.0;

        m_nAcceleration.x = 0.0;
        m_nAcceleration.y = 0.0;

        m_tAccel = 0.0;
        m_tConstVelocity = 0.0;
        m_tFinish = 0.0;
        return;
    }
    m_tAccel = fRate / m_Acceleration;
    float min_dist = m_Acceleration * m_tAccel * m_tAccel;
    if(min_dist >= distance)
    {
        //need to calculate a new max speed and accel/decel time
        fRate =  vsqrtf( distance * m_Acceleration );
        m_tAccel = fRate / m_Acceleration;
        m_tConstVelocity = 0.0;
    }
    else
    {
        //calculate the constant velocity time
        float remaining = distance - m_Acceleration * m_tAccel * m_tAccel;
        m_tConstVelocity = remaining /fRate;
    }

    m_nTargetPos = nTargetPos;

    //save the target position and the target rate
    m_nTargetPos = nTargetPos;

    m_nTargetVelocity.x = fRate * delta_pos.x / distance;
    m_nTargetVelocity.y = fRate * delta_pos.y / distance;

    m_nAcceleration.x = m_Acceleration * delta_pos.x / distance;
    m_nAcceleration.y = m_Acceleration * delta_pos.y / distance;

    //setup the state transition times
    m_tConstVelocity += m_tAccel;
    m_tFinish = m_tAccel + m_tConstVelocity;

    m_nStep = START_MOVE;
}

bool CPathLine::RunMove(float dT, FloatVector &position,  FloatVector &velocity,  FloatVector &acceleration)
{
    m_nTime+=dT;
    switch(m_nStep)
    {
        case START_MOVE:
        {
            //for now set the velocity to zero, until the planning has velocity matching
            velocity.x = 0.0;
            velocity.y = 0.0;
            acceleration = m_nAcceleration;
            m_nStep = ACCELERATION;
            //intentional fall through
        }
        case ACCELERATION:
        {
            if(m_nTime <= m_tAccel)
            {
                velocity = velocity + acceleration * dT;
                position = position + velocity * dT;
                break;
            }
            velocity = m_nTargetVelocity;
            acceleration.x = 0.0;
            acceleration.y = 0.0;
            m_nStep = CONSTANT_VELOC;
        }
        case CONSTANT_VELOC:
        {
            if(m_nTime <= m_tConstVelocity)
            {
                position = position + velocity * dT;
                break;
            }
            acceleration.x = -m_nAcceleration.x;
            acceleration.y = -m_nAcceleration.y;
            m_nStep = DECELERATION;
        }
        case DECELERATION:
        {

            if(m_nTime <= m_tFinish)
            {
                velocity = velocity + acceleration * dT;
                position = position + velocity * dT;
                break;
            }
            acceleration.x = 0.0;
            acceleration.y = 0.0;
            velocity.x = 0.0;
            velocity.y = 0.0;
            position = m_nTargetPos;
            return true;
        }
    }
    return false;
}




void CPathArc::SetUpMove(FloatVector nInitPos, FloatVector nTargetPos, FloatVector nCenterPoint,float fRate, bool bClockwise)
{
    m_nFinishPosition = nTargetPos;
    m_nCenterPoint = nCenterPoint;

    //calculate the radius from the center and target position
    FloatVector delta_finish =  nTargetPos - nCenterPoint;
    m_radius = vsqrtf(delta_finish.x*delta_finish.x + delta_finish.y*delta_finish.y);

    //calculate the starting angle, and finish angle
    FloatVector delta_start =  nInitPos - nCenterPoint;
    m_angle = atan2f(float(delta_start.y),float(delta_start.x));
    m_finish_angle = atan2f(float(delta_finish.y),float(delta_finish.x));

    //calculate the arc distance traveled
    float fDeltaAngle = m_angle - m_finish_angle;
    if(bClockwise)
    {
        if(m_angle < m_finish_angle)
        //if(m_angle > m_finish_angle)
        {
            fDeltaAngle = 2 * PI + fDeltaAngle;
        }
    }
    else
    {
        if(m_angle > m_finish_angle)
        //if(m_angle < m_finish_angle)
        {
            fDeltaAngle = -2 * PI + fDeltaAngle;
        }
    }
    float distance = fDeltaAngle;
    if(distance < 0)
    {
        distance = -distance;
    }

    //calculate the angular rate
    fRate = fRate/m_radius;

    //calculate the movement times
    m_tAccel = fRate / m_Acceleration;
    float min_dist = m_Acceleration * m_tAccel * m_tAccel;
    if(min_dist >= distance)
    {
        //need to calculate a new max speed and accel/decel time
        fRate =  vsqrtf( distance * m_Acceleration );
        m_tAccel = fRate / m_Acceleration;
        m_tConstVelocity = 0.0;
    }
    else
    {
        //calculate the constant velocity time
        float remaining = distance - m_Acceleration * m_tAccel * m_tAccel;
        m_tConstVelocity = remaining /fRate;
    }

    //from the radius and direction, calculate the angular rate and acceleration
    //C=2*PI*r
    m_TargetAngularRate = fRate;
    m_AngularAcceleration = m_Acceleration/m_radius;
    if(bClockwise)
    {
        m_TargetAngularRate = -m_TargetAngularRate;
        m_AngularAcceleration = -m_AngularAcceleration;
    }

    m_nTime = 0.0;
    m_AngularRate = 0.0;
    //setup the state transition times
    m_tConstVelocity += m_tAccel;
    m_tFinish = m_tAccel + m_tConstVelocity;

    m_nStep = START_MOVE;
}

bool CPathArc::RunMove(float dT,  FloatVector &position,  FloatVector &velocity,  FloatVector &acceleration)
{
    m_nTime+=dT;
    float angular_acceleration = 0.0;
    switch(m_nStep)
    {
        case START_MOVE:
        {
            //for now set the velocity to zero, until the planning has velocity matching
            velocity.x = 0.0;
            velocity.y = 0.0;
            m_nStep = ACCELERATION;
            //intentional fall through
        }
        case ACCELERATION:
        {
            if(m_nTime <= m_tAccel)
            {
                m_AngularRate += m_AngularAcceleration * dT;
                m_angle += m_AngularRate * dT;
                break;
            }
            m_AngularRate = m_TargetAngularRate;
            angular_acceleration = 0.0;
            m_nStep = CONSTANT_VELOC;
        }
        case CONSTANT_VELOC:
        {
            if(m_nTime <= m_tConstVelocity)
            {
                m_angle += m_AngularRate * dT;
                break;
            }
            m_nStep = DECELERATION;
        }
        case DECELERATION:
        {
            if(m_nTime <= m_tFinish)
            {
                m_AngularRate -= m_AngularAcceleration * dT;
                m_angle += m_AngularRate * dT;
                break;
            }
            acceleration.x = 0.0;
            acceleration.y = 0.0;
            velocity.x = 0.0;
            velocity.y = 0.0;
            position = m_nFinishPosition;
            return true;
        }
    }


    //function to calculate (x,y) position, velocity, acceleration
    float sin_angle = sinf(m_angle);
    float cos_angle = cosf(m_angle);
    position.x = cos_angle * m_radius + m_nCenterPoint.x;
    position.y = sin_angle * m_radius + m_nCenterPoint.y;
    velocity.x = -sin_angle * m_radius * m_AngularRate;
    velocity.y = cos_angle * m_radius * m_AngularRate;
    acceleration.x = -sin_angle * m_radius * angular_acceleration;
    acceleration.y = cos_angle * m_radius * angular_acceleration;

    return false;
}





void CommandList::AddPathToBuffer(CommandBase *pBaseCommand)
{
    if(pBaseCommand != NULL)
    {
        //return the starting point to its list
        switch(pBaseCommand->m_CommandType)
        {
            case CommandBase::POINT_PATH:
            {
                AddPathToBuffer(static_cast<CPathPoint*>(pBaseCommand));
                break;
            }
            case CommandBase::LINE_PATH:
            {
                AddPathToBuffer(static_cast<CPathLine*>(pBaseCommand));
                break;
            }
            case CommandBase::ARC_PATH:
            {
                AddPathToBuffer(static_cast<CPathArc*>(pBaseCommand));
                break;
            }
            case CommandBase::LASER_POWER:
            {
                AddPathToBuffer(static_cast<CLaserPower*>(pBaseCommand));
                break;
            }
            case CommandBase::LASER_ON_OFF:
            {
                AddPathToBuffer(static_cast<CLaserOnOff*>(pBaseCommand));
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

CommandBase *CommandList::GetNextPath()
{
    CommandBase *pResp = m_pStart;
    if(pResp != NULL)
    {
        m_pStart = m_pStart->m_pNext;
        if(m_pStart == NULL)
        {
            m_pFinish = NULL;
        }
    }
    return pResp;
}



void CommandList::AddToEnd(CommandBase *pPath)
{
    pPath->m_pNext = NULL;
    if(m_pStart == NULL)
    {
        m_pStart = pPath;
        m_pFinish = pPath;
    }
    else
    {
        m_pFinish->m_pNext = pPath;
        m_pFinish = pPath;
    }
}


void CommandList::AddPathToBuffer(CPathPoint *pPointPath)
{
    m_nPointNum++;
    if(m_pPointList == NULL)
    {
        m_pPointList = pPointPath;
        pPointPath->m_pNext = NULL;
    }
    else
    {
        pPointPath->m_pNext = m_pPointList;
        m_pPointList = pPointPath;
    }
}

void CommandList::AddPathToBuffer(CPathLine *pLinePath)
{
    m_nLineNum++;
    if(m_pLineList == NULL)
    {
        m_pLineList = pLinePath;
        pLinePath->m_pNext = NULL;
    }
    else
    {
        pLinePath->m_pNext = m_pLineList;
        m_pLineList = pLinePath;
    }
}

void CommandList::AddPathToBuffer(CPathArc *pArcPath)
{
    m_nArcNum++;
    if(m_pArcList == NULL)
    {
        m_pArcList = pArcPath;
        pArcPath->m_pNext = NULL;
    }
    else
    {
        pArcPath->m_pNext = m_pArcList;
        m_pArcList = pArcPath;
    }
}

void CommandList::AddPathToBuffer(CLaserOnOff *pLaserControl)
{
    m_nLaserOnOffNum++;
    if(m_pLaserOnOffList == NULL)
    {
        m_pLaserOnOffList = pLaserControl;
        pLaserControl->m_pNext = NULL;
    }
    else
    {
        pLaserControl->m_pNext = m_pLaserOnOffList;
        m_pLaserOnOffList = pLaserControl;
    }
}

void CommandList::AddPathToBuffer(CLaserPower *pBaseCommand)
{
    m_nLaserPowerNum++;
    if(m_pLaserPowerList == NULL)
    {
        m_pLaserPowerList = pBaseCommand;
        pBaseCommand->m_pNext = NULL;
    }
    else
    {
        pBaseCommand->m_pNext = m_pLaserPowerList;
        m_pLaserPowerList = pBaseCommand;
    }
}

//return false if not added
bool CommandList::AddArc(FloatVector nInitPos,
        FloatVector nTargetPos,
        FloatVector nCenterPoint,
        float fRate,
        bool bClockwise,
        bool deferedAck)
{
    if(m_pArcList == NULL)
    {
        return false;
    }
    m_nArcNum--;
    CPathArc *pArcPath = m_pArcList;
    m_pArcList = static_cast<CPathArc *>(pArcPath->m_pNext);
    pArcPath->SetUpMove(nInitPos,nTargetPos,nCenterPoint,fRate,bClockwise);
    pArcPath->m_DeferedAck = deferedAck;
    AddToEnd(pArcPath);
    return true;
}

bool CommandList::AddPath(FloatVector nInitPos,
        FloatVector nTargetPos,
        float fRate,
        bool deferedAck)
{
    if(m_pLineList == NULL)
    {
        return false;
    }
    m_nLineNum--;
    CPathLine *pLinePath = m_pLineList;
    m_pLineList = static_cast<CPathLine *>(pLinePath->m_pNext);
    pLinePath->SetUpMove(nInitPos,nTargetPos,fRate);
    pLinePath->m_DeferedAck = deferedAck;
    AddToEnd(pLinePath);
    return true;
}

bool CommandList::AddPoint(FloatVector nInitPos,
        float fDelay,
        bool deferedAck)
{
    if(m_pPointList == NULL)
    {
        return false;
    }
    m_nPointNum--;
    CPathPoint *pPointPath = m_pPointList;
    m_pPointList = static_cast<CPathPoint *>(pPointPath->m_pNext);
    pPointPath->SetUpMove(nInitPos,fDelay);
    pPointPath->m_DeferedAck = deferedAck;
    AddToEnd(pPointPath);
    return true;
}

bool CommandList::AddLaserPower(float fPower,
        bool deferedAck)
{
    if(m_pLaserPowerList == NULL)
    {
        return false;
    }
    m_nPointNum--;
    CLaserPower *pLaserCommand = m_pLaserPowerList;
    m_pLaserPowerList = static_cast<CLaserPower *>(pLaserCommand->m_pNext);
    pLaserCommand->m_bLaserPower = fPower;
    pLaserCommand->m_DeferedAck = deferedAck;
    AddToEnd(pLaserCommand);
    return true;
}

bool CommandList::AddLaserOnOff(bool bLaserOn,
        bool deferedAck)
{
    if(m_pLaserOnOffList == NULL)
    {
        return false;
    }
    m_nPointNum--;
    CLaserOnOff *pLaserCommand = m_pLaserOnOffList;
    m_pLaserOnOffList = static_cast<CLaserOnOff *>(pLaserCommand->m_pNext);
    pLaserCommand->m_bLaserOn = bLaserOn;
    pLaserCommand->m_DeferedAck = deferedAck;
    AddToEnd(pLaserCommand);
    return true;

}
