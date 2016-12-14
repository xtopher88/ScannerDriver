/*
 * Base.h
 *
 *  Created on: Dec 12, 2016
 *      Author: kchristopher
 */

#ifndef SOURCE_BASE_H_
#define SOURCE_BASE_H_

#include "Include.h"
#include "Scheduler.h"


namespace nsBaseClass
{
    enum DataType
    {
        DATA_NONE,
        DATA_UINT,
        DATA_INT,
        DATA_FLOAT
    };

    enum TimerType
    {
        TIMER_POLING,
        TIMER_TIMEOUT
    };

    enum STATUS_CODES
    {
        STATUS_OK,
        STATUS_ERROR
    };
}


class BaseClass
{
public:
    // register the class with the scheduler
    BaseClass(unsigned int nID);
    /// Initialization function called by the scheduler
    virtual void Init()=0;
    /// Timer function called every timer tick update
    virtual void Timer(unsigned int nTicks){}
    /// run command function is used to generically call functions from other devices
    virtual unsigned int Command(unsigned int nCmd,
            void *pData,
            unsigned int &nData,
            unsigned int nDataType){ return nsBaseClass::STATUS_ERROR;}

protected:

    friend class Scheduler;
    BaseClass *m_pNext;
    unsigned int m_nID;
};



#endif /* SOURCE_BASE_H_ */
