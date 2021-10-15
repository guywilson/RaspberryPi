#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "peripheral.h"
#include "exception.h"

BCM238X_Peripheral::BCM238X_Peripheral(int fd, uint32_t baseAddress)
{
	map = mmap(
				0,
				BLOCK_SIZE,
				PROT_READ | PROT_WRITE,
				MAP_SHARED,
				fd,
				baseAddress);

	if (map < (void *)0) {
		throw new Exception(
					ERR_MEMORY_MAP_ERROR,
					"Failed to map peripheral to system memory",
					__FILE__,
					"BCM238X_Peripheral",
					"BCM238X_Peripheral()",
					__LINE__);
	}
}

BCM238X_Peripheral::~BCM238X_Peripheral()
{
	munmap((uint32_t *)map, BLOCK_SIZE);
}

volatile void * BCM238X_Peripheral::getMemoryMap()
{
	return map;
}


GPIOMemory_Peripheral::GPIOMemory_Peripheral(int fd, uint32_t baseAddress) : BCM238X_Peripheral(fd, baseAddress)
{
	checkPrivileges();
}

void GPIOMemory_Peripheral::checkPrivileges()
{
	/*
	** No need to check anything, can be accessed
	** without using sudo...
	*/
	return;
}


ProtectedMemory_Peripheral::ProtectedMemory_Peripheral(int fd, uint32_t baseAddress) : BCM238X_Peripheral(fd, baseAddress)
{
	checkPrivileges();
}

void ProtectedMemory_Peripheral::checkPrivileges()
{
	/*
	** Must check privileges, as it must be accessed
	** using sudo...
	*/
	if (geteuid() != 0) {
		throw new Exception(
					ERR_INSUFFICIENT_PRIVILEGES,
					"Unable to access protected memory, must run as root user",
					__FILE__,
					"ProtectedMemory_Peripheral",
					"checkPrivileges()",
					__LINE__);
	}
}
