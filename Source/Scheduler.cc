/*
 * Scheduler.cc
 *
 *  Created on: Dec 13, 2016
 *      Author: kchristopher
 */


#include "Scheduler.h"

Scheduler::Scheduler()
{
    m_pFirstDevice = NULL;
    m_nTicks = 0;
    m_nLastTick = 0;
}

// Register devices with the scheduler when constructed
void Scheduler::RegisterDevice(BaseClass *pDevice)
{
    // set the first device
    if(m_pFirstDevice == NULL)
    {
        m_pFirstDevice = pDevice;
    }
    else
    {
        // go through the linked list to find the last device and set its next to the current
        BaseClass *pLastDevice = m_pFirstDevice;
        while(pLastDevice->m_pNext != NULL)
        {
            pLastDevice = pLastDevice->m_pNext;
        }
        pLastDevice->m_pNext = pDevice;
    }
    // ensure that the current next device is NULL
    pDevice->m_pNext = NULL;
}

// Initialize all devices in the order they were constructed
void Scheduler::Init()
{
    BaseClass *pDevice = m_pFirstDevice;
    while(pDevice != NULL)
    {
        pDevice->Init();
        pDevice = pDevice->m_pNext;
    }
}

// the send command is used to route commands based in the ID number
unsigned int Scheduler::SendCommand(
        unsigned int nID,
        unsigned int nCmd,
        void *pData,
        unsigned int &nData,
        unsigned int &nDataType)
{
    BaseClass *pDevice = m_pFirstDevice;
    while(pDevice != NULL)
    {
        if(pDevice->m_nID == nID)
        {
            return pDevice->Command(nCmd, pData, nData, nDataType);
        }
    }
    return nsBaseClass::STATUS_ERROR;
}

//
void Scheduler::TimerUpdate()
{
    m_nTicks++;
}

// the scheduler
void Scheduler::RunScheduler()
{
    while(1)
    {
        if(m_nTicks != m_nLastTick)
        {
            BaseClass *pDevice = m_pFirstDevice;
            while(pDevice != NULL)
            {
                pDevice->Timer(m_nTicks);
            }
            m_nLastTick = m_nTicks;
        }
    }
}

