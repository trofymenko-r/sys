/*
 * PhysBus.h
 *
 *  Created on: 5 квіт. 2017
 *      Author: root
 */

#pragma once

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "types.h"

namespace sys {

class CPhysBus
{
public:
   CPhysBus();
   ~CPhysBus();

private:
   int fd;

public:
   bool Init();
   U32 Read(U32 u32Addr);
   bool Read(U32 u32Addr, U8* pu8Data, U32 u32Bytes);
   bool Write(U32 u32Addr, U32 u32Data);
   bool Write(U32 u32Addr, U8* pu8Data, U32 u32Bytes);
};

} /* namespace sys */
