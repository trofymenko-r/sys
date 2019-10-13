/*
 * threadsafe_queue.h
 *
 *  Created on: 28 вер. 2016
 *      Author: root
 */


#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace sys {

template<typename T> class threadsafe_queue_group
{
public:
   mutable std::mutex mut;
   std::queue<std::shared_ptr<T> > data_queue;
   std::condition_variable data_cond;
   //std::condition_variable queue_avail;
   unsigned int uiMaxSize;

public:
   threadsafe_queue_group(){}
   threadsafe_queue_group(unsigned int uiMaxSize):uiMaxSize(uiMaxSize){}

//   void wait_and_pop(T& value)
//   {
//      std::unique_lock<std::mutex> lk(mut);
//      data_cond.wait(lk,[this]{return !data_queue.empty();});
//      value=std::move(*data_queue.front());
//      data_queue.pop();
//   }

//   bool try_pop(T& value)
//   {
//      std::lock_guard<std::mutex> lk(mut);
//
//      if(data_queue.empty())
//         return false;
//      value=std::move(*data_queue.front());
//      data_queue.pop();
//      return true;
//   }

//   void wait_data()
//   {
//      std::unique_lock<std::mutex> lk(mut);
//      data_cond.wait(lk,[this]{return !data_queue.empty();});
//   }

   std::shared_ptr<T> /*wait_and_*/pop()
   {
//      std::unique_lock<std::mutex> lk(mut);
//      data_cond.wait(lk,[this]{return !data_queue.empty();});
      std::shared_ptr<T> res=data_queue.front();
      data_queue.pop();

      return res;
   }

//   std::shared_ptr<T> try_pop()
//   {
//      std::lock_guard<std::mutex> lk(mut);
//      if(data_queue.empty())
//         return std::shared_ptr<T>();
//      std::shared_ptr<T> res=data_queue.front();
//      data_queue.pop();
//      return res;
//   }

//   void lock()
//   {
//      mut.lock();
//   }
//
//   void unlock()
//   {
//      mut.unlock();
//   }
//
//   void notify()
//   {
//      data_cond.notify_all();
//   }

   bool push(T new_value)
   {
      std::shared_ptr<T> data(
            std::make_shared<T>(std::move(new_value)));
      //std::lock_guard<std::mutex> lk(mut);

      if(uiMaxSize>0 && data_queue.size()>=uiMaxSize) return false;

      data_queue.push(data);
      //data_cond.notify_all();
      return true;
   }

//   bool empty() const
//   {
//      std::lock_guard<std::mutex> lk(mut);
//      return data_queue.empty();
//   }
//
//   int size()
//   {
//      std::lock_guard<std::mutex> lk(mut);
//      return data_queue.size();
//   }
};

} /* namespace sys */
