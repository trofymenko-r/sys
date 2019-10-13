/*
 * threadsafequeue.h
 *
 *  Created on: 25 серп. 2016
 *      Author: root
 */

#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

namespace sys {

template<typename T>
class CMCThreadSafeQueue
{
public:
   CMCThreadSafeQueue()
         :
               head(new node),
               tail(head.get()),
               uiSize(0),
               uiMaxSize(0)
   {
   }

   CMCThreadSafeQueue(uint uiMaxSize_)
         :
               head(new node),
               tail(head.get()),
               uiSize(0),
               uiMaxSize(uiMaxSize_)
   {
   }

   CMCThreadSafeQueue(const CMCThreadSafeQueue& other) = delete;

private:
   struct node
   {
      std::shared_ptr<T> data;
      std::unique_ptr<node> next;
   };

   mutex head_mutex;
   mutex head_mutex1;
   unique_ptr<node> head;
   mutex tail_mutex;
   node* tail;
   condition_variable data_cond;
   condition_variable queue_avail;
   uint uiSize;
   uint uiMaxSize;

public:
   CMCThreadSafeQueue& operator=(const CMCThreadSafeQueue& other) = delete;

private:
   node* get_tail()
   {
      std::lock_guard < std::mutex > tail_lock(tail_mutex);
      return (tail);
   }

   std::unique_ptr<node> pop_head()
   {
      std::unique_ptr<node> old_head = std::move(head);
      head = std::move(old_head->next);
      std::unique_lock < std::mutex > head_lock(head_mutex1); //??
      if(uiSize > 0) uiSize--;
      queue_avail.notify_one();
      return (old_head);
   }

   void wait_queue_avail()
   {
      std::unique_lock < std::mutex > head_lock(head_mutex1);
      queue_avail.wait(head_lock, [&]{return uiSize < uiMaxSize;});
   }

   std::unique_lock<std::mutex> wait_for_data()
   {
      std::unique_lock < std::mutex > head_lock(head_mutex);
      data_cond.wait(head_lock, [&]{return head.get()!=get_tail();});
      return std::move(head_lock);
   }

   std::unique_ptr<node> wait_pop_head()
   {
      std::unique_lock < std::mutex > head_lock(wait_for_data());
      return pop_head();
   }

   std::unique_ptr<node> wait_pop_head(T& value)
         {
      std::unique_lock < std::mutex > head_lock(wait_for_data());
      value = std::move(*head->data);
      return pop_head();
   }

   std::unique_ptr<node> try_pop_head()
   {
      std::lock_guard < std::mutex > head_lock(head_mutex);
      if(head.get() == get_tail()){
         return std::unique_ptr<node>();
      }
      return pop_head();
   }

   std::unique_ptr<node> try_pop_head(T& value)
         {
      std::lock_guard < std::mutex > head_lock(head_mutex);
      if(head.get() == get_tail()){
         return std::unique_ptr<node>();
      }

      value = std::move(*head->data);
      return pop_head();
   }

public:
   void push(T new_value)
   {
      //if(uiMaxSize>0 && uiSize>=uiMaxSize) return;

      std::shared_ptr<T> new_data(
            std::make_shared < T > (std::move(new_value)));

      std::unique_ptr < node > p(new node);
      {
         std::lock_guard < std::mutex > tail_lock(tail_mutex);
         tail->data = new_data;
         node* const new_tail = p.get();
         tail->next = std::move(p);
         tail = new_tail;
         uiSize++;
      }

      data_cond.notify_one();
   }

   void wait_and_push(T new_value)
   {
      if(uiMaxSize>0) wait_queue_avail();
      push(new_value);
   }

   std::shared_ptr<T> wait_and_pop()
   {
      std::unique_ptr<node> const old_head = wait_pop_head();
      return old_head->data;
   }

   void wait_and_pop(T& value)
   {
      std::unique_ptr<node> const old_head = wait_pop_head(value);
   }

   std::shared_ptr<T> try_pop()
   {
      std::unique_ptr<node> old_head = try_pop_head();
      return old_head ? old_head->data : std::shared_ptr<T>();
   }

   bool try_pop(T& value)
         {
      std::unique_ptr<node> const old_head = try_pop_head(value);
      return old_head;
   }

   bool empty()
   {
      std::lock_guard < std::mutex > head_1ock(head_mutex);
      return (head.get() == get_tail());
   }

   bool full()
   {
      return (uiSize == uiMaxSize);
   }

   uint size()
   {
      return uiSize;
   }
};

} /* namespace sys */
