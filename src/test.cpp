#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <ctype.h>

#include "pifactory.h"
#include "rasppi.h"
#include "swtimer.h"
#include "exception.h"

void testFlashOutput22(RaspberryPi * pi);
void testPullUpInput22(RaspberryPi * pi);
void testPullDownInput22(RaspberryPi * pi);
void testPullUpEventRising22(RaspberryPi * pi);
void testPullUpEventFalling22(RaspberryPi * pi);
void testClock4(RaspberryPi * pi);
void testPWM18(RaspberryPi * pi);


int main(int argc, char *argv[])
{
	RaspberryPi	*	pi;
	uint32_t		fnSelectOrig[6];
	int				i;
	int				mode = 0;
	char			szArg[32];
	
	if (argc == 2) {
		if (strlen(argv[1]) > 2) {
			printf("Error - expecting a mode number argument\n");
			exit(-1);
		}
		
		strcpy(szArg, argv[1]);
		
		for (i = 0;i < (int)strlen(szArg);i++) {
			if (!isdigit(szArg[i])) {
				printf("Expecting numeric mode argument\n");
				exit(-1);
			}
		}
		
		mode = atoi(szArg);;
	}
	else if (argc > 2) {
		printf("Error - expecting only 1 numeric mode argument\n");
		exit(-1);
	}
		
	printf("Running in mode %d\n", mode);

	try {
		/*
		** Make the correct flavour of Pi...
		*/
		PiFactory factory = PiFactory::getInstance();
		pi = factory.makePi();
	}
	catch (Exception * e) {
		printf("Caught exception instantiating RaspberryPi: %s\n\n", e->getExceptionString());
		return(-1);
	}
	
	printf("Made the pi...\n");

	/*
	** Get the original state of the FnSelect registers...
	*/
	try {
		for (i = 0;i < 6;i++) {
			fnSelectOrig[i] = pi->getGpio()->getGPFSELN(i);
		}
	}
	catch (Exception * e) {
		printf("Caught exception getting FnSelect: %s\n\n", e->getExceptionString());
		return(-1);
	}

	switch (mode) {
		case 1:
			/*
			** Detect input on pin 22 - use pull up resistor and short 22 to GND
			*/
			testPullUpInput22(pi);
			break;
			
		case 2:
			/*
			** Detect input on pin 22 - use pull down resistor and connect 22 to 3.3V
			*/
			testPullDownInput22(pi);
			break;
			
		case 3:
			/*
			** Detect rising edge event on pin 22 - use pull up resistor and short 22 to GND
			*/
			testPullUpEventRising22(pi);
			break;
			
		case 4:
			/*
			** Detect rising edge event on pin 22 - use pull up resistor and short 22 to GND
			*/
			testPullUpEventFalling22(pi);
			break;
			
		case 5:
			/*
			** Output clock on pin 4
			*/
			testClock4(pi);
			break;
			
		case 6:
			/*
			** Test PWM on pin 18
			*/
			testPWM18(pi);
			break;
			
		default:
			/*
			** Default mode - flash pin 22 on & off - connect LED+Res/multimeter from 22 to GND
			*/
			testFlashOutput22(pi);
			break;
	}
	
	/*
	** Put the FnSelect registers back as they were...
	*/
	try {
		for (i = 0;i < 6;i++) {
			pi->getGpio()->setGPFSELN(i, fnSelectOrig[i]);
		}
	}
	catch (Exception * e) {
		printf("Caught exception setting FnSelect: %s\n\n", e->getExceptionString());
		return(-1);
	}
	
	delete pi;

	return 0;
}

void testFlashOutput22(RaspberryPi * pi)
{
	int i;
	
	try {
		GPIO * pGPIO = pi->getGpio();

		printf("Got GPIO...\n");
		
		pGPIO->setPinFunction(22, fnOutput);

		for (i = 0;i < 20;i++) {
			pGPIO->setPin(22);
			printf("Set pin 22 [0x%02X]...\n", pGPIO->getPinLevel(22));
			
			SWTimer::ssDelay(1);
			
			pGPIO->clrPin(22);
			printf("Clear pin 22 [0x%02X]...\n", pGPIO->getPinLevel(22));
			
			SWTimer::ssDelay(1);
		}
	}
	catch (Exception * e) {
		printf("Caught exception in testFlashOutput22(): %s\n\n", e->getExceptionString());
		exit(-1);
	}
}

void testPullUpInput22(RaspberryPi * pi)
{
	try {
		GPIO * pGPIO = pi->getGpio();

		pGPIO->setPinFunction(22, fnInput);
		
		pGPIO->setPinPullUpDown(22, updnUp);
		
		byte lastLevel = 0x04;
		byte b = 0x00;
		int count = 0;
		
		while (count < 20) {
			b = pGPIO->getPinLevel(22);
			SWTimer::usDelay(50);
			b = pGPIO->getPinLevel(22);
			SWTimer::usDelay(50);
			b = pGPIO->getPinLevel(22);
			
			if (b != lastLevel) {
				printf("Pin 22 is 0x%02X\n", b);
				lastLevel = b;
				count++;
			}
			
			SWTimer::msDelay(1);
		}

		pGPIO->setPinPullUpDown(22, updnOff);
	}
	catch (Exception * e) {
		printf("Caught exception in testPullUpInput22(): %s\n\n", e->getExceptionString());
		exit(-1);
	}
}

void testPullDownInput22(RaspberryPi * pi)
{
	try {
		GPIO * pGPIO = pi->getGpio();

		pGPIO->setPinFunction(22, fnInput);
		
		pGPIO->setPinPullUpDown(22, updnDown);
		
		byte lastLevel = 0x04;
		byte b = 0x00;
		int count = 0;
		
		while (count < 20) {
			b = pGPIO->getPinLevel(22);
			SWTimer::msDelay(1);
			b = pGPIO->getPinLevel(22);
			SWTimer::msDelay(1);
			b = pGPIO->getPinLevel(22);
			
			if (b != lastLevel) {
				printf("Pin 22 is 0x%02X\n", b);
				lastLevel = b;
				count++;
			}
		}

		pGPIO->setPinPullUpDown(22, updnOff);
	}
	catch (Exception * e) {
		printf("Caught exception in testPullDownInput22(): %s\n\n", e->getExceptionString());
		exit(-1);
	}
}

void testPullUpEventRising22(RaspberryPi * pi)
{
	byte	status = 0x00;
	int		count = 0;

	try {
		GPIO * pGPIO = pi->getGpio();

		pGPIO->setPinFunction(22, fnInput);
		
		pGPIO->setPinPullUpDown(22, updnUp);
		
		pGPIO->setPinEventDetect(22, evSyncRisingEdge);
		
		while (count < 20) {
			status = pGPIO->getPinEventStatus(22);
			
			if (status) {
				printf("Pin 22 rising edge detected\n");
				
				SWTimer::msDelay(100);
				
				pGPIO->clearPinEventStatus(22);
				
				count++;
			}
			
			SWTimer::msDelay(5);
		}

		pGPIO->clearPinEventDetect(22, evSyncRisingEdge);

		pGPIO->setPinPullUpDown(22, updnOff);
	}
	catch (Exception * e) {
		printf("Caught exception in testPullUpEventRising22(): %s\n\n", e->getExceptionString());
		exit(-1);
	}
}

void testPullUpEventFalling22(RaspberryPi * pi)
{
	byte	status = 0x00;
	int		count = 0;

	try {
		GPIO * pGPIO = pi->getGpio();

		pGPIO->setPinFunction(22, fnInput);
		
		pGPIO->setPinPullUpDown(22, updnUp);
		
		pGPIO->setPinEventDetect(22, evSyncFallingEdge);
		
		while (count < 20) {
			status = pGPIO->getPinEventStatus(22);
			
			if (status) {
				printf("Pin 22 falling edge detected\n");
				
				SWTimer::msDelay(100);
				
				pGPIO->clearPinEventStatus(22);
				
				count++;
			}
			
			SWTimer::msDelay(5);
		}

		pGPIO->clearPinEventDetect(22, evSyncFallingEdge);

		pGPIO->setPinPullUpDown(22, updnOff);
	}
	catch (Exception * e) {
		printf("Caught exception in testPullUpEventFalling22(): %s\n\n", e->getExceptionString());
		exit(-1);
	}
}

void testClock4(RaspberryPi * pi)
{
	try {
		GPIO * pGPIO = pi->getGpio();
		CLOCK * pClock = pi->getClock();
		
		// Set GPIO pin 4 as GPCLK0...
		pGPIO->setPinFunction(4, fnAlt0);
		
		pClock->setFrequency(clk0, 4688);
		
		pClock->start(clk0);
		
		SWTimer::ssDelay(5);
		
		pClock->stop(clk0);
	}
	catch (Exception * e) {
		printf("Caught exception in testClock4(): %s\n\n", e->getExceptionString());
		exit(-1);
	}
}

void testPWM18(RaspberryPi * pi)
{
	uint32_t	pwmCtrlFlags;
	uint32_t	i;

	try {
		GPIO * pGPIO = pi->getGpio();
		PWM * pPWM = pi->getPwm();
		
		// Set GPIO pin 18 as PWM channel 0...
		pGPIO->setPinFunction(18, fnAlt5);

		// Stop PWM prior to changing the clock frequency...
		pPWM->disable();
		
		pwmCtrlFlags =	PWM_CTRL_MODE_PWM |
						PWM_CTRL_REPEAT_INT |
						PWM_CTRL_SILENCE_OFF |
						PWM_CTRL_POLARITY_HIGH |
						PWM_CTRL_FIFO_DISABLE |
						PWM_CTRL_ALGORITHM_PWM;
		
		pPWM->setup(PWM_CHANNEL_0, pwmCtrlFlags, 600000L, 1024);
		
		pPWM->enable(PWM_CHANNEL_0);
		
		for (i = 16;i <= 4096;i += 16) {
			SWTimer::msDelay(15);
			pPWM->setData(PWM_CHANNEL_0, i);
			printf("+");
			fflush(stdout);
		}
		
		printf("\n\n");
		
		SWTimer::ssDelay(1);

		for (i = 4096;i >= 16;i -= 16) {
			SWTimer::msDelay(15);
			pPWM->setData(PWM_CHANNEL_0, i);
			printf("-");
			fflush(stdout);
		}
		
		printf("\n\n");

		// Stop PWM channel 0...
		pPWM->disable(PWM_CHANNEL_0);
	}
	catch (Exception * e) {
		printf("Caught exception in testPWM18(): %s\n\n", e->getExceptionString());
		exit(-1);
	}
}