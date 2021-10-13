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
	public:
		enum Manufacturer {
			SonyUK = 0,
			Egoman = 1,
			Embest = 2,
			SonyJapan = 3,
			Stadium = 5,
			Qisda = 100			// Only for the first Rpis using the old encoding scheme
		};

		enum MemorySize {
			_256Mb = 0,
			_512Mb = 1,
			_1Gb = 2,
			_2Gb = 3,
			_4Gb = 4,
			_8Gb = 5
		};

		enum Processor {
			BCM2835 = 0,
			BCM2836 = 1,
			BCM2837 = 2,
			BCM2711 = 3
		};

		enum Model {
			Rpi_Model_A = 			0x00,
			Rpi_Model_B = 			0x01,
			Rpi_Model_A_plus = 		0x02,
			Rpi_Model_B_plus  = 	0x03,
			Rpi_Model_2B = 			0x04,
			Alpha_prototype = 		0x05,
			Compute_Module1 = 		0x06,
			Rpi_Model_3B = 			0x08,
			Rpi_Zero = 				0x09,
			Compute_Module3 = 		0x0A,
			Rpi_ZeroW = 			0x0C,
			Rpi_Model_3B_plus = 	0x0D,
			Rpi_Model_3A_plus = 	0x0E,
			Internal_use_only = 	0x0F,
			Compute_Module3_plus = 	0x10,
			Rpi_Model_4B = 			0x11,
			Rpi_Model_400 = 		0x13,
			Compute_Module4 = 		0x14
		};

	private:
		CLOCK	*			pCLOCK	= nullptr;
		GPIO	*			pGPIO 	= nullptr;
		SPI		*			pSPI 	= nullptr;
		PWM		*			pPWM 	= nullptr;
		Manufacturer		manufacturer;
		MemorySize			memSize;
		Processor			processor;
		Model				model;

		char				szRevision[8];
		char *				pszInfo = nullptr;
		
		int					memFd = 0;
		
		volatile uint32_t	pPeriBase;
		
		void				_setMaxPriority();
		
	public:
		RaspberryPi(
				Model m, 
				MemorySize s, 
				Processor p, 
				Manufacturer mfr, 
				char * pszRevision, 
				uint32_t baseAddress);

		~RaspberryPi();
		
		uint32_t			getBaseAddress();

		CLOCK	*			getClock();
		GPIO	*			getGpio();
		SPI		*			getSpi();
		PWM		*			getPwm();

		Manufacturer		getManufacturer();
		MemorySize			getMemorySize();
		Processor			getProcessor();
		Model				getModel();
		int					getRevision();

		char *				getInfoString();
};

#endif
