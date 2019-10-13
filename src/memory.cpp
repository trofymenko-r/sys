/*
 * memory.cpp
 *
 *  Created on: 29 ���. 2015
 *      Author: Igor
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

#include "memory.h"

#define MAP_SIZE           (4096)
#define MAP_MASK           (MAP_SIZE-1)


bool readmem(IN 	uint32_t* 	pu32Addr,
		   IN	uint32_t    u32Words,
		   OUT 	void*		pBuffer
		   )
{
	int fd;

	fd = open( "/dev/mem", O_RDWR | O_SYNC );
	if(fd < 0) return false;

	readmem(pu32Addr, u32Words, pBuffer, fd);

	close(fd);

	return true;
}

bool readmem(IN 	uint32_t* 	pu32Addr,
		   IN	uint32_t    u32Words,
		   OUT 	void*		pBuffer,
		   IN	int fd
		   )
{
	if((u32Words*4) > MAP_SIZE) return false;

	void* pvmap_page_addr;
	void* pvmap_addr;
	off_t addr = (off_t)pu32Addr;

	pvmap_page_addr = mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr&~MAP_MASK);
	if(pvmap_page_addr==MAP_FAILED) return false;

	pvmap_addr = pvmap_page_addr + (addr&MAP_MASK);

	for(U32 ii=0; ii<u32Words; ii++){
		*((uint32_t*)pBuffer+ii) = *((uint32_t*)pvmap_addr+ii);
	}

	if(munmap(pvmap_page_addr, MAP_SIZE)) return false;

	return true;
}

bool writemem(IN 	uint32_t* 	pu32Addr,
		   IN	uint32_t    u32Words,
		   IN 	void*		pBuffer
		   )
{
	int fd;

	fd = open( "/dev/mem", O_RDWR | O_SYNC );
	if(fd < 0) return false;

	writemem(pu32Addr, u32Words, pBuffer, fd);

	close(fd);

	return true;
}

bool writemem(IN 	uint32_t* 	pu32Addr,
		   IN	uint32_t    u32Words,
		   IN 	void*		pBuffer,
		   IN	int fd
		   )
{
	if((u32Words*4) > MAP_SIZE) return false;

	void *pvmap_page_addr;
	void *pvmap_addr;
	off_t addr = (off_t)pu32Addr;

	pvmap_page_addr = mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr&~MAP_MASK);
	if(pvmap_page_addr==MAP_FAILED) return false;

	pvmap_addr = pvmap_page_addr + (addr&MAP_MASK);

	for(U32 ii=0; ii<u32Words; ii++){
		*((uint32_t*)pvmap_addr+ii) = *((uint32_t*)pBuffer+ii);
	}

	if(munmap(pvmap_page_addr, MAP_SIZE)) return false;

	return true;
}


