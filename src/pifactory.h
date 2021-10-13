
#ifndef _INC_PIFACTORY
#define _INC_PIFACTORY

#include "rasppi.h"

class PiFactory
{
	private:
		static uint32_t _getRevision();
		
	public:
		static RaspberryPi * makePi();
};

#endif