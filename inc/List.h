#pragma once

#include "types.h"
#include "func.h"
#include <cstring>

namespace global{

template <class Type, U16 u16MaxSize> class CList
{
public:
   CList():u16Size(0)
   {
      for(U16 ii=0; ii<u16MaxSize; ii++)
         memclr(&Items[ii], sizeof(Type));
   }
   
protected:
   U16 u16Size;
   
protected:
   Type Items[u16MaxSize];
   
public:
   inline bool Add(Type Item)
   {
      return push_back(Item);
   }

   inline bool push_back(Type Item)
   {
      return(insert(Item, u16Size));
   }
   
   bool Insert(Type Item, U16 u16Pos)
   {
      return insert(Item, u16Pos);
   }

   bool insert(Type Item, U16 u16Pos)
   {
      if(u16Pos >= u16MaxSize) return(false);
      else if(u16Pos < u16Size){
         for(U16 ii=u16Size; ii>u16Pos; ii--){
            Items[ii] = Items[ii-1];
         }
      }
      //Items[u16Pos] = Item;
      memcpy(&Items[u16Pos], &Item, sizeof(Type));
      if(u16Size < u16MaxSize) u16Size++;
      
      return(true);
   }
   
   bool Remove(U16 u16Pos)
   {
      return remove(u16Pos);
   }

   bool remove(U16 u16Pos)
   {
      if(u16Pos >= u16Size) return(false);
      
      for(U16 ii=u16Pos; ii<(u16Size-1); ii++){
         Items[ii] = Items[ii+1];
      }
      u16Size--;
      memclr(&Items[u16Size], sizeof(Type));
      
      return(true);
   }
   
   inline void Clear()
   {
      clear();
   }

   inline void clear(void)
   {
      for(U16 ii=0; ii<u16Size; ii++)
         memclr(&Items[ii], sizeof(Type));
      u16Size = 0;
   }
   
   inline bool empty()
   {
      return(u16Size==0);
   }

   inline bool full()
   {
      return(u16Size==u16MaxSize);
   }

   inline void FastClear(void){u16Size = 0;}
   
   inline S32 Find(Type Item)
   {
      return find(Item);
   }

   S32 find(Type Item)
   {
      for(U16 ii=0; ii<u16Size; ii++){
         if(memcmp(&Items[ii], &Item, sizeof(Type))==0)
            return((S32)ii);
      }
      
      return(-1);
   }
   
   inline U16 Size() const
   {
      return size();
   }

   inline U16 size() const
   {
      return(u16Size);
   }
   
   U16 MaxSize() const
   {
      return(u16MaxSize);
   }
   
   inline Type* Item(U16 u16Pos)
   {
      return(&Items[u16Pos]);
   }

public:
   inline Type operator[](U16 u16Pos) const
   {
      return(Items[u16Pos]);
   }

   CList& operator=(const CList& right){
      if(this==&right) {
         return *this;
      }
      memcpy(this->Items, right.Items, sizeof(Type)*right.u16Size);
      this->u16Size = right.u16Size;
      return *this;
   }
};

typedef CList<U32, 8> U32List8;
typedef CList<U32, 16> U32List16;
typedef CList<U32, 32> U32List32;

}; /* namespace global */
