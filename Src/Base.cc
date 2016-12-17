/*
 * base.cc
 *
 *  Created on: Dec 13, 2016
 *      Author: kchristopher
 */

#include "Base.h"

BaseClass::BaseClass(unsigned int nID)
{
    m_pNext = NULL;
    m_nID = nID;
    Scheduler::getScheduler().RegisterDevice(this);
}


