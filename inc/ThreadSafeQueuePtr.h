/*
 * ThreadSafeQueuePtr.h
 *
 *  Created on: 28 вер. 2016
 *      Author: root
 */


#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace sys {

template<typename T> class CThreadSafeQueuePtr
{
private:
   mutable std::mutex mut;
   std::queue<std::shared_ptr<T> > data_queue;
   //std::queue<T> data_queue;
   std::condition_variable data_cond;
   //std::condition_variable queue_avail;
   unsigned int uiMaxSize;

public:
   CThreadSafeQueuePtr():uiMaxSize(0){}
   CThreadSafeQueuePtr(unsigned int uiMaxSize):uiMaxSize(uiMaxSize){}

   void wait_and_pop(T& value)
   {
      std::unique_lock<std::mutex> lk(mut);
      data_cond.wait(lk,[this]{return !data_queue.empty();});
      value=std::move(*data_queue.front());
      data_queue.pop();
      //queue_avail.notify_all();
   }

   bool try_pop(T& value)
   {
      std::lock_guard<std::mutex> lk(mut);

      if(data_queue.empty())
         return false;
      value=std::move(*data_queue.front());
      data_queue.pop();
      return true;
   }

   std::shared_ptr<T> wait_and_pop()
   {
      std::unique_lock<std::mutex> lk(mut);
      data_cond.wait(lk,[this]{return !data_queue.empty();});
      std::shared_ptr<T> res=data_queue.front();
      data_queue.pop();
      //queue_avail.notify_all();

      return res;
   }

   std::shared_ptr<T> try_pop()
   {
      std::lock_guard<std::mutex> lk(mut);
      if(data_queue.empty())
         return std::shared_ptr<T>();
      std::shared_ptr<T> res=data_queue.front();
      data_queue.pop();
      return res;
   }

//   bool Avail()
//   {
//      return(data_queue.size() < iMaxSize);
//   }

//   bool push(T new_value)
//   {
//      //std::lock_guard<std::mutex> lk(mut);
//      std::unique_lock<std::mutex> lk(mut);
//      queue_avail.wait(lk,[this]{return Avail();});
//      //if(iMaxSize>0 && data_queue.size()>=iMaxSize) return false;
//
//      std::shared_ptr<T> data(
//            std::make_shared<T>(std::move(new_value)));
//      std::lock_guard<std::mutex> lk1(mut);
//
//      data_queue.push(data);
//      data_cond.notify_all();
//      return true;
//   }

//   void wait_and_push(T new_value)
//   {
//      std::unique_lock<std::mutex> lk(mut);
//      queue_avail.wait(lk,[this]{return Avail();});
//
//      std::shared_ptr<T> data(
//            std::make_shared<T>(std::move(new_value)));
//      data_queue.push(data);
//      //push(new_value);
//
//   }

   bool push(T& new_value)
   {
//      std::shared_ptr<T> data(
//            std::make_shared<T>(std::move(new_value)));
      std::lock_guard<std::mutex> lk(mut);

      if(uiMaxSize>0 && data_queue.size()>=uiMaxSize) return false;

      std::shared_ptr<T> data(
                  std::make_shared<T>(std::move(new_value)));

      data_queue.push(data);
      data_cond.notify_all();
      return true;
   }

   bool push(std::vector<T>& values)
   {
      std::lock_guard<std::mutex> lk(mut);
      bool bAdded = false;
      U32 ii = 0;

      auto value = values.begin();
      //for(auto& value: values){
      while(value!=values.end()){
         T& data1 = (T&)*value;
         std::shared_ptr<T> data(
               std::make_shared<T>(std::move(data1/*(T&)*value*/)));

         if(uiMaxSize>0 && data_queue.size()>=uiMaxSize)
            break;

         data_queue.push(data);
         values.erase(values.begin());
         bAdded = true;
         //ii++;
      }

      /*if(bAdded)*/ data_cond.notify_all();

      return true;
   }

   bool empty() const
   {
      std::lock_guard<std::mutex> lk(mut);
      return data_queue.empty();
   }

   int size()
   {
      std::lock_guard<std::mutex> lk(mut);
      return data_queue.size();
   }
};

} /* namespace sys */
