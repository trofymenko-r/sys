/*
 * CThread.h
 *
 *  Created on: 13 серп. 2016
 *      Author: root
 */

#pragma once

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <signal.h>
#endif

namespace sys{

#ifdef WIN32
typedef HANDLE ThreadType;
#else
typedef pthread_t ThreadType;
#endif

class CThread
{
public:
   CThread();
   virtual ~CThread();

private:
   ThreadType __handle;
   CThread(const CThread&);
   void operator=(const CThread&);

public:
   void Start();
   void Start(void* Func(void *));
   virtual void Execute() = 0;
   void Join();
   void Kill();
};

} //sys

