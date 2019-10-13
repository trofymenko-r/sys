/*
 * Thread.cpp
 *
 *  Created on: 13 серп. 2016
 *      Author: root
 */

#include <cstdlib>
#include "Thread.h"

namespace sys{

static void ThreadCallback(CThread* pThread) {
#ifndef WIN32
   int old_thread_type;
   pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_thread_type);
#endif
   pThread->Execute();
}

CThread::CThread()
{
   __handle = (ThreadType)nullptr;

}

#ifdef WIN32

CThread::~CThread(){
  CloseHandle(__handle);
}

void CThread::Start(){
   __handle = CreateThread(0,
                           0,
                           reinterpret_cast<LPTHREAD_START_ROUTINE>(ThreadCallback),
                           this,
                           0,
                           0);
}

void CThread::Join() {
   WaitForSingleObject(__handle,  INFINITE);
}

void CThread::Kill(){
   TerminateThread(__handle, 0);
}

#else
CThread::~CThread()
{

}

extern "C"
typedef void *(*pthread_callback)(void *);

void CThread::Start(){
   pthread_create(&__handle,
                  0,
                  reinterpret_cast<pthread_callback>(ThreadCallback),
                  this);
}

void CThread::Start(void* Func(void *)){  //void (*Func)(void) (*__start_routine)
   pthread_create(&__handle,
                  0,
                  reinterpret_cast<pthread_callback>(Func),
                  this);
}

void CThread::Join(){
   pthread_join(__handle, 0);
}

void CThread::Kill(){
   pthread_cancel(__handle);
}

#endif

} //sys
