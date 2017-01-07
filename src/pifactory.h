
#ifndef _INC_PIFACTORY
#define _INC_PIFACTORY

#include "rasppi.h"

class PiFactory
{
	private:
		static int _getModel();
		
	public:
		static RaspberryPi * makePi();
};

#endif