
#ifndef _INC_CLOCK
#define _INC_CLOCK

#include "peripheral.h"

typedef enum 
{
	clk0,
	clk1,		// Reserved for system use...
	clk2,
	clkPWM
}
ClockId;

class CLOCK : public ProtectedMemory_Peripheral
{
	protected:
		bool			isBusy(ClockId id);
		void			waitWhileBusy(ClockId id);
		
	public:
		CLOCK(int memFd, uint32_t baseAddress);
		
		void			start(ClockId id);
		void			stop(ClockId id);
		
		unsigned long	getFrequency(ClockId id);
		void			setFrequency(ClockId id, unsigned long frequency);
		
		uint32_t		getCLKMGR_0();
		void			setCLKMGR_0(uint32_t value);
		
		uint32_t		getCLKDIV_0();
		void			setCLKDIV_0(uint32_t value);
		
		/*
		** Clock 1 is reserved for system use...
		*/
/* 		uint32_t		getCLKMGR_1();
		void			setCLKMGR_1(uint32_t value);
		
		uint32_t		getCLKDIV_1();
		void			setCLKDIV_1(uint32_t value);
*/
		uint32_t		getCLKMGR_2();
		void			setCLKMGR_2(uint32_t value);
		
		uint32_t		getCLKDIV_2();
		void			setCLKDIV_2(uint32_t value);
 		
		uint32_t		getCLKMGR_PWM();
		void			setCLKMGR_PWM(uint32_t value);
		
		uint32_t		getCLKDIV_PWM();
		void			setCLKDIV_PWM(uint32_t value);
};

#endif
