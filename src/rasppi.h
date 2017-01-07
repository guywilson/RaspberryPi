#include <stdint.h>

#ifndef _INC_RASPPI
#define _INC_RASPPI

#include "gpio.h"
#include "clock.h"
#include "pwm.h"
#include "spi.h"

#define GPIO_TIMER	  		0x0000B000	/* Timer */
#define UART0_BASE			0x00201000	/* Uart 0 */
#define UART1_BASE			0x00215000	/* Uart 1 (not used) */

class RaspberryPi
{
	private:
		CLOCK	*			pCLOCK	= nullptr;
		GPIO	*			pGPIO 	= nullptr;
		SPI		*			pSPI 	= nullptr;
	//	UART	*			pUART 	= nullptr;
		PWM		*			pPWM 	= nullptr;
		
		int					memFd = 0;
		
		volatile uint32_t	pPeriBase;
		
		void				_setMaxPriority();
		
	protected:
		RaspberryPi(uint32_t baseAddress);
		
		uint32_t			getBaseAddress();
		
	public:
		~RaspberryPi();
		
		CLOCK	*	getClock();
		GPIO	*	getGpio();
		SPI		*	getSpi();
	//	UART	*	getUart();
		PWM		*	getPwm();
};

class RaspberryPiOriginal : public RaspberryPi
{
	public:
		RaspberryPiOriginal(uint32_t baseAddress);
};

class RaspberryPiNew : public RaspberryPi
{
	public:
		RaspberryPiNew(uint32_t baseAddress);
};

#endif
