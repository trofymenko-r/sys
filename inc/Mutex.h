/*
 * Mutex.h
 *
 *  Created on: 23 серп. 2016
 *      Author: root
 */

#pragma once

#include <pthread.h>

namespace sys {

class CMutex
{
public:
   CMutex();
   virtual ~CMutex();

private:
   pthread_mutex_t __handle;

public:
   bool Lock();
   bool UnLock();
};

} /* namespace sys */
