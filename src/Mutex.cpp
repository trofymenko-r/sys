/*
 * Mutex.cpp
 *
 *  Created on: 23 серп. 2016
 *      Author: root
 */

#include "Mutex.h"

namespace sys {

CMutex::CMutex()
{
   pthread_mutex_init(&__handle, NULL);
}

CMutex::~CMutex()
{
   pthread_mutex_destroy(&__handle);
}

bool CMutex::Lock()
{
   int ret = pthread_mutex_lock(&__handle);
   if(ret==0)  return(true);
   else        return(false);
}

bool CMutex::UnLock()
{
   int ret = pthread_mutex_unlock(&__handle);
   if(ret==0)  return(true);
   else        return(false);
}

} /* namespace sys */
