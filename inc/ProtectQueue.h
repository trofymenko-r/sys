/*
 * ProtectQueue.h
 *
 *  Created on: 23 серп. 2016
 *      Author: root
 */

#pragma once

#include <mutex>
#include "Queue.h"
#include "Mutex.h"

using namespace global;
using namespace std;

namespace sys{

template <class Type, U16 u16MaxSize> class CProtectQueue: public CQueue<Type, u16MaxSize>
{
#define Parent  CQueue<Type, u16MaxSize>
public:
   CProtectQueue():Parent()
   {

   }
   virtual ~CProtectQueue(){}


private:
   mutex HeadMutex;
   //CMutex HeadMutex;
   //CMutex aMutex[u16MaxSize];

private:
   inline U16 FrontIndex(void)
   {
      return(0);
   }

   inline U16 BackIndex(void)
   {
      return(0);
   }

public:
   //pthread_mutex_t GetMutex

   inline bool push(Type Item)
   {
      lock_guard<mutex> lock(HeadMutex);
      return(Parent::push(Item, false));
   }

   inline void pop()
   {
      lock_guard<mutex> lock(HeadMutex);
      Parent::pop();
   }

   inline void clear(void)
   {
      lock_guard<mutex> lock(HeadMutex);
      Parent::clear();
   }

   inline void FastClear(void)
   {
      lock_guard<mutex> lock(HeadMutex);
      Parent::FastClear();
   }

   inline Type* Item(U16 u16Pos)
   {
      lock_guard<mutex> lock(HeadMutex);
      return(Parent::Item(u16Pos));
   }

   inline Type* front(void)
   {
      lock_guard<mutex> lock(HeadMutex);
      return(Parent::front());
   }

   inline Type* back(void)
   {
      lock_guard<mutex> lock(HeadMutex);
      return(Parent::back());
   }

public:
   inline Type operator[](U16 u16Pos)
   {
      lock_guard<mutex> lock(HeadMutex);
      return(Parent::operator[]);
   }

#undef Parent
};

} /* namespace sys */
