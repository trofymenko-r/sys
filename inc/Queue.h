/*
 * Queue.h
 *
 *  Created on: 19 серп. 2016
 *      Author: root
 */

#pragma once

#include "List.h"

namespace global{

template <class Type, U16 u16MaxSize> class CQueue: public CList<Type, u16MaxSize>
{
#define Parent  CList<Type, u16MaxSize>
public:
   CQueue():Parent()
   {
      ClearData();
   }

private:
   U16 u16Back;
   U16 u16Front;
   U16 u16Rings;
   static const U16 u16NoValidIndex = 0xFFFF;


private:
   bool Replace(Type Item, U16 u16Pos)
   {
      if(u16Pos>=u16MaxSize || u16Pos>=Parent::u16Size) return(false);
      Parent::Items[u16Pos] = Item;
      return(true);
   }

   inline bool push_back(Type Item)
   {
      return(push(Item));
   }

   void ClearData()
   {
      u16Back = u16NoValidIndex;
      u16Front = u16NoValidIndex;
      u16Rings = 0;
   }

public:
   inline bool push(Type Item)
   {
      return(push(Item, true));
   }

   inline bool push(Type Item, bool bExtrudeFront)
   {
      if(Parent::u16Size < u16MaxSize){
         Parent::push_back(Item);
         if(u16Front==u16NoValidIndex) u16Front = 0;
         if(u16Back==u16NoValidIndex) u16Back = 0;
         else u16Back++;
      }else{
         if(!bExtrudeFront) return(false);

         if(u16Back==(u16MaxSize-1)){
            u16Back = 0;
            u16Front = 1;
            u16Rings++;
         }else{
            if(u16Back==(u16MaxSize-2)) u16Front = 0;
            else u16Front++;
            u16Back++;
         }

         Replace(Item, u16Back);
      }

      return(true);
   }

   inline void pop()
   {
      if(Parent::u16Size==0) return;

      //Parent::Items[u16Front] = 0; //debug

      if(u16Front < u16Back){
         u16Front++;
      }else if(u16Front==u16Back){
         ClearData();
      }else{ //u16Front >u16Back
         if(u16Front < (u16MaxSize-1)) u16Front++;
         else u16Front = 0;
      }

      Parent::u16Size--;
   }

   inline void clear(void)
   {
      Parent::clear();
      ClearData();
   }

   inline void FastClear(void)
   {
      Parent::FastClear();
      ClearData();
   }

   inline Type* Item(U16 u16Pos)
   {
      U16 u16RealPos = (u16Front + u16Pos) % u16MaxSize;
      return(&Parent::Items[u16RealPos]);
   }

   inline Type* front(void)
   {
      return(&Parent::Items[u16Front]);
   }

   inline Type* back(void)
   {
      return(&Parent::Items[u16Back]);
   }

   inline U16 FrontIndex(void)
   {
      return(u16Front);
   }

   inline U16 BackIndex(void)
   {
      return(u16Back);
   }

public:
   inline Type operator[](U16 u16Pos)
   {
      U16 u16RealPos = (u16Front + u16Pos) % u16MaxSize;
      return(Parent::Items[u16RealPos]);
   }
#undef Parent
};

} /* namespace global */
