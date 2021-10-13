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


RaspberryPi::RaspberryPi(				
				Model m, 
				MemorySize s, 
				Processor p, 
				Manufacturer mfr, 
				char * pszRevision, 
				uint32_t baseAddress)
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
	
	this->model = m;
	this->memSize = s;
	this->manufacturer = mfr;
	this->processor = p;

	strcpy(szRevision, pszRevision);

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

char * RaspberryPi::getInfoString()
{
	if (this->pszInfo == nullptr) {
		pszInfo = (char *)malloc(128);

		if (pszInfo == NULL) {
			throw new Exception("Error allocating memory");
		}
	}

	switch (model) {
		case Rpi_Model_A:
			strcpy(pszInfo, "Raspberry Pi Model A");
			break;

		case Rpi_Model_A_plus:
			strcpy(pszInfo, "Raspberry Pi Model A+");
			break;

		case Rpi_Model_B:
			strcpy(pszInfo, "Raspberry Pi Model B");
			break;

		case Rpi_Model_B_plus:
			strcpy(pszInfo, "Raspberry Pi Model B+");
			break;

		case Rpi_Model_2B:
			strcpy(pszInfo, "Raspberry Pi Model 2B");
			break;

		case Rpi_Model_3A_plus:
			strcpy(pszInfo, "Raspberry Pi Model 3A+");
			break;

		case Rpi_Model_3B:
			strcpy(pszInfo, "Raspberry Pi Model 3B");
			break;

		case Rpi_Model_3B_plus:
			strcpy(pszInfo, "Raspberry Pi Model 3B+");
			break;

		case Rpi_Model_400:
			strcpy(pszInfo, "Raspberry Pi Model 400");
			break;

		case Rpi_Model_4B:
			strcpy(pszInfo, "Raspberry Pi Model 4B");
			break;

		case Rpi_Zero:
			strcpy(pszInfo, "Raspberry Pi Zero");
			break;

		case Rpi_ZeroW:
			strcpy(pszInfo, "Raspberry Pi ZeroW");
			break;

		case Compute_Module1:
			strcpy(pszInfo, "Raspberry Pi Compute Module 1");
			break;

		case Compute_Module3:
			strcpy(pszInfo, "Raspberry Pi Compute Module 3");
			break;

		case Compute_Module3_plus:
			strcpy(pszInfo, "Raspberry Pi Compute Module 3+");
			break;

		case Compute_Module4:
			strcpy(pszInfo, "Raspberry Pi Compute Module 4");
			break;

		case Alpha_prototype:
			strcpy(pszInfo, "Raspberry Pi Alpha prototype");
			break;

		case Internal_use_only:
			strcpy(pszInfo, "Model: Internal Use Only");
			break;

		default:
			strcpy(pszInfo, "ERROR: Unknown model");
			break;
	}

	strcat(pszInfo, " rev");
	strcat(pszInfo, szRevision);
	strcat(pszInfo, " with ");

	switch (memSize) {
		case _256Mb:
			strcat(pszInfo, "256Mb RAM");
			break;

		case _512Mb:
			strcat(pszInfo, "512Mb RAM");
			break;

		case _1Gb:
			strcat(pszInfo, "1024Mb RAM");
			break;

		case _2Gb:
			strcat(pszInfo, "2048Mb RAM");
			break;

		case _4Gb:
			strcat(pszInfo, "4096Mb RAM");
			break;

		case _8Gb:
			strcat(pszInfo, "8192Mb RAM");
			break;

		default:
			strcat(pszInfo, "ERROR: Unknown memory size");
			break;
	}

	strcat(pszInfo, ", manufactured by ");

	switch (manufacturer) {
		case SonyUK:
			strcat(pszInfo, "Sony UK");
			break;

		case SonyJapan:
			strcat(pszInfo, "Sony Japan");
			break;

		case Egoman:
			strcat(pszInfo, "Egoman");
			break;

		case Embest:
			strcat(pszInfo, "Embest");
			break;

		case Stadium:
			strcat(pszInfo, "Stadium");
			break;

		case Qisda:
			strcat(pszInfo, "Qisda");
			break;

		default:
			strcat(pszInfo, "ERROR: Unknown manufacturer");
			break;
	}

	return pszInfo;
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
