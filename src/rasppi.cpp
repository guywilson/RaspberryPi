#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "rasppi.h"
#include "exception.h"

#define PROCESS_PRIORITY			55


RaspberryPi::RaspberryPi(uint32_t baseAddress)
{
	memFd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
	
	if (memFd < 0) {
		memFd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC);
	}
	
	if (memFd < 0) {
		throw new Exception(
					ERR_DEVICE_OPEN_FAILURE,
					"Faied to open /dev/gpiomem",
					__FILE__,
					"RaspberryPi",
					"RaspberryPi()",
					__LINE__);
	}
	
	pPeriBase = baseAddress;
	
	_setMaxPriority();
}

RaspberryPi::~RaspberryPi()
{
	if (this->pGPIO != nullptr) {
		delete pGPIO;
	}

	if (this->pCLOCK != nullptr) {
		delete pCLOCK;
	}

	if (this->pPWM != nullptr) {
		delete pPWM;
	}
}

void RaspberryPi::_setMaxPriority()
{
#ifndef __APPLE__
	struct sched_param	sched;
	int					ret;
	int					maxPriority;

	memset(&sched, 0, sizeof(sched));

	if (geteuid() == 0) {
		maxPriority = sched_get_priority_max(SCHED_RR);

		if (maxPriority < PROCESS_PRIORITY) {
			sched.sched_priority = maxPriority;
		}
		else {
			sched.sched_priority = PROCESS_PRIORITY;
		}

		ret = sched_setscheduler(0, SCHED_RR, &sched);
		
		if (ret < 0) {
			throw new Exception(
						ERR_SCHEDULE,
						"Faied to set scheduler priority",
						__FILE__,
						"RaspberryPi",
						"_setMaxPriority()",
						__LINE__);
		}
	}
#endif
}

uint32_t RaspberryPi::getBaseAddress()
{
	return pPeriBase;
}

GPIO * RaspberryPi::getGpio()
{
	if (this->pGPIO == nullptr) {
		this->pGPIO = new GPIO(memFd, pPeriBase);
	}
	
	return pGPIO;
}

CLOCK * RaspberryPi::getClock()
{
	if (this->pCLOCK == nullptr) {
		this->pCLOCK = new CLOCK(memFd, pPeriBase);
	}
	
	return pCLOCK;
}

PWM * RaspberryPi::getPwm()
{
	if (this->pPWM == nullptr) {
		this->pPWM = new PWM(memFd, pPeriBase);
		pPWM->setClock(getClock());
	}
	
	return pPWM;
}

SPI * RaspberryPi::getSpi()
{
	if (this->pSPI == nullptr) {
		this->pSPI = new SPI(memFd, pPeriBase);
	}
	
	return pSPI;
}


RaspberryPiOriginal::RaspberryPiOriginal(uint32_t baseAddress) : RaspberryPi(baseAddress)
{
	// Nowt to do...
}


RaspberryPiNew::RaspberryPiNew(uint32_t baseAddress) : RaspberryPi(baseAddress)
{
	// Nowt to do...
}
