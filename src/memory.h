/*
 * memory.h
 *
 *  Created on: 29 ���. 2015
 *      Author: Igor
 */

#pragma once

#include "types.h"

bool readmem(IN 	uint32_t* 	pu32Addr,
		   IN	uint32_t    u32Words,
		   OUT 	void*		pBuffer
		   );

bool readmem(IN 	uint32_t* 	pu32Addr,
		   IN	uint32_t    u32Words,
		   OUT 	void*		pBuffer,
		   IN	int fd
		   );

bool writemem(IN 	uint32_t* 	pu32Addr,
		   IN	uint32_t    u32Words,
		   IN 	void*		pBuffer
		   );

bool writemem(IN 	uint32_t* 	pu32Addr,
		   IN	uint32_t    u32Words,
		   IN 	void*		pBuffer,
		   IN	int fd
		   );


