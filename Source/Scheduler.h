/*
 * Scheduler.h
 *
 *  Created on: Dec 12, 2016
 *      Author: kchristopher
 */

#ifndef SOURCE_SCHEDULER_H_
#define SOURCE_SCHEDULER_H_

#include "Base.h"

class BaseClass;

class Scheduler
{
public:
    Scheduler();

    // Use singleton to ensure there is only one instance of the scheduler class
    static Scheduler& getScheduler()
    {
        static Scheduler m_Scheduler;
        return m_Scheduler;
    }

    // Register devices with the scheduler when constructed
    void RegisterDevice(BaseClass *pDevice);

    // Initialize all devices in the order they were constructed
    void Init();

    // the send command is used to route commands based in the ID number
    unsigned int SendCommand(
            unsigned int nID,
            unsigned int nCmd,
            void *pData,
            unsigned int &nData,
            unsigned int &nDataType);

    //
    void TimerUpdate();

    // the scheduler
    void RunScheduler();
protected:
    BaseClass *m_pFirstDevice;

    unsigned int m_nTicks;
    unsigned int m_nLastTick;
};



#endif /* SOURCE_SCHEDULER_H_ */
