
#ifndef _INC_PIFACTORY
#define _INC_PIFACTORY

#include "rasppi.h"

class PiFactory
{
	private:
		PiFactory();
		
		RaspberryPi *				pi = nullptr;

		uint32_t _getRevision();
		
	public:
		static PiFactory & getInstance() {
			static PiFactory instance;

			return instance;
		}

		~PiFactory();

		RaspberryPi * makePi();
};

#endif