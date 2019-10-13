/*
 * PhysBus.cpp
 *
 *  Created on: 5 квіт. 2017
 *      Author: root
 */

#include "PhysBus.h"
#include "memory.h"

namespace sys {

CPhysBus::CPhysBus():fd(0)
{

}

CPhysBus::~CPhysBus()
{
   close(fd);
}

/******************************************************************************/
/*  Init                                                                      */
/******************************************************************************/
bool CPhysBus::Init()
{
   fd = open("/dev/mem", O_RDWR | O_SYNC );
   if(fd < 0){
      printf( "error open /dev/mem");
      return false;
   }

   return true;
}

/******************************************************************************/
/*  Read                                                                      */
/******************************************************************************/
U32 CPhysBus::Read(U32 u32Addr)
{
   U32 u32Word;
   readmem((uint32_t*)u32Addr, 1, &u32Word, fd);

   return u32Word;
}

/******************************************************************************/
/*  Read                                                                      */
/******************************************************************************/
bool CPhysBus::Read(U32 u32Addr, U8* pu8Data, U32 u32Bytes)
{
   return readmem((uint32_t*)u32Addr, u32Bytes>>2, pu8Data, fd);
}

/******************************************************************************/
/*  Write                                                                      */
/******************************************************************************/
bool CPhysBus::Write(U32 u32Addr, U32 u32Data)
{
   return writemem((uint32_t*)u32Addr, 1, &u32Data, fd);
}

/******************************************************************************/
/*  Write                                                                      */
/******************************************************************************/
bool CPhysBus::Write(U32 u32Addr, U8* pu8Data, U32 u32Bytes)
{
   return writemem((uint32_t*)u32Addr, u32Bytes>>2, pu8Data, fd);
}

} /* namespace sys */
