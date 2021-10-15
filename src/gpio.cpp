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

#include "gpio.h"
#include "types.h"
#include "register.h"
#include "swtimer.h"

#define GPIO_BASE	  		0x00200000

volatile GPIO_MAP * gpioMap;


GPIO::GPIO(int memFd, uint32_t baseAddress) : GPIOMemory_Peripheral(memFd, baseAddress + GPIO_BASE)
{
	int				i;
	
	printf("In GPIO constructor...\n");
	fflush(stdout);

	gpioMap = (GPIO_MAP *)getMemoryMap();
	
	for (i = 0;i < 54;i++) {
		pullUpDownState[i] = updnOff;
	}
}

int GPIO::_getTwinRegisterNum(int pinNum)
{
	return pinNum > 31 ? 1 : 0;
}

uint32_t GPIO::_getTwinRegisterMask(int pinNum)
{
	uint32_t mask = 0x00000000;
	
	mask = 1 << (pinNum > 31 ? (pinNum - 32) : pinNum);
	
	return mask;
}

PinFunction GPIO::getPinFunction(int pinNum)
{
	int				registerNum = 0;
	int				shift = 0;
	uint32_t		currentMask = 0x00000000;
	PinFunction		function;
	
	registerNum = pinNum / 10;
	shift = (pinNum % 10) * 3;
	
	currentMask = getGPFSELN(registerNum);
	
	function = (PinFunction)(((currentMask & (7 << shift)) >> shift) & 0xFF);
	
	return function;
}

void GPIO::setPinFunction(int pinNum, PinFunction function)
{
	int				registerNum = 0;
	int				shift = 0;
	uint32_t		newMask = 0x00000000;
	uint32_t		currentMask = 0x00000000;
	
	registerNum = pinNum / 10;
	shift = (pinNum % 10) * 3;
	
	currentMask = getGPFSELN(registerNum);

	if (function == fnInput) {
		newMask = currentMask & (~(7 << shift));
	}
	else {
		newMask = (currentMask & (~(7 << shift))) | (function << shift);
	}
	
	setGPFSELN(registerNum, newMask);
}

PullUpDn GPIO::getPinPullUpDown(int pinNum)
{
	return pullUpDownState[pinNum];
}

/*
	1.	Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
		to remove the current Pull-up/down)
	2. 	Wait 150 cycles – this provides the required set-up time for the control signal
	3. 	Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
		modify – NOTE only the pads which receive a clock will be modified, all others will
		retain their previous state.
	4. 	Wait 150 cycles – this provides the required hold time for the control signal
	5. 	Write to GPPUD to remove the control signal
	6. 	Write to GPPUDCLK0/1 to remove the clock
	
	With a clock speed of 700MHz:
		Clock cycle = 0.0014us. 
		150 cycles = 0.2us
		
	With a clock speed of 900MHz:
		Clock cycle = 0.0011us. 
		150 cycles = 0.17us
*/
void GPIO::setPinPullUpDown(int pinNum, PullUpDn pullUpDown)
{
	int			registerNum = 0;
	uint32_t	mask;
	
	/*
	** Remember the pull up/down state for the pin...
	*/
	pullUpDownState[pinNum] = pullUpDown;
	
	registerNum = _getTwinRegisterNum(pinNum);
	mask = _getTwinRegisterMask(pinNum);
	
	setGPPUD((uint32_t)pullUpDown);
	
	SWTimer::usDelay(10);
	setGPPUDCLKN(registerNum, mask);
	SWTimer::usDelay(10);
	
	setGPPUD((uint32_t)updnOff);

	setGPPUDCLKN(registerNum, 0x00);
}

byte GPIO::getPinLevel(int pinNum)
{
	int				registerNum = 0;
	uint32_t		reg = 0x00000000;
	uint32_t		mask = 0x00000000;
	byte			level = 0x00;
	
	registerNum = _getTwinRegisterNum(pinNum);
	mask = _getTwinRegisterMask(pinNum);

	reg = getGPLEVN(registerNum);
	reg &= mask;
	
	level = reg > 0 ? 1 : 0;
	
	return level;
}

byte GPIO::getPinEventStatus(int pinNum)
{
	byte		status;
	int			registerNum;
	uint32_t	mask = 0x00000000;
	uint32_t	value = 0x00000000;
	
	registerNum = _getTwinRegisterNum(pinNum);
	mask = _getTwinRegisterMask(pinNum);

	value = getGPEDSN(registerNum);
	value &= mask;
	
	status = value > 0 ? 1 : 0;
	
	return status;
}

void GPIO::clearPinEventStatus(int pinNum)
{
	int			registerNum;
	uint32_t	mask = 0x00000000;
	
	registerNum = _getTwinRegisterNum(pinNum);
	mask = _getTwinRegisterMask(pinNum);

	setGPEDSN(registerNum, mask);
}

/*
** Sets the event detect registers, e.g. rising/falling edge and level
** When an event occurs that has been registered for, the corresponding
** bit will be set in the event status register for that pin...
*/
void GPIO::setPinEventDetect(int pinNum, PinEvent event)
{
	int			registerNum;
	uint32_t	value = 0x00000000;
	
	registerNum = _getTwinRegisterNum(pinNum);
	value = _getTwinRegisterMask(pinNum);
	
	switch (event) {
		case evSyncRisingEdge:
			setGPRENN(registerNum, value);
			break;

		case evSyncFallingEdge:
			setGPFENN(registerNum, value);
			break;

		case evSyncBoth:
			setGPRENN(registerNum, value);
			setGPFENN(registerNum, value);
			break;

		case evAsyncRisingEdge:
			setGPARENN(registerNum, value);
			break;

		case evAsyncFallingEdge:
			setGPAFENN(registerNum, value);
			break;

		case evAsyncBoth:
			setGPARENN(registerNum, value);
			setGPFENN(registerNum, value);
			break;

		case evLevelHigh:
			setGPHENN(registerNum, value);
			break;

		case evLevelLow:
			setGPLENN(registerNum, value);
			break;
	}
}

void GPIO::clearPinEventDetect(int pinNum, PinEvent event)
{
	int			registerNum;
	uint32_t	value = 0x00000000;
	
	registerNum = _getTwinRegisterNum(pinNum);
	value = ~(_getTwinRegisterMask(pinNum));
	
	switch (event) {
		case evSyncRisingEdge:
			setGPRENN(registerNum, getGPRENN(registerNum) & value);
			break;

		case evSyncFallingEdge:
			setGPFENN(registerNum, getGPFENN(registerNum) & value);
			break;

		case evSyncBoth:
			setGPRENN(registerNum, getGPRENN(registerNum) & value);
			setGPFENN(registerNum, getGPFENN(registerNum) & value);
			break;

		case evAsyncRisingEdge:
			setGPARENN(registerNum, getGPARENN(registerNum) & value);
			break;

		case evAsyncFallingEdge:
			setGPAFENN(registerNum, getGPAFENN(registerNum) & value);
			break;

		case evAsyncBoth:
			setGPARENN(registerNum, getGPARENN(registerNum) & value);
			setGPAFENN(registerNum, getGPAFENN(registerNum) & value);
			break;

		case evLevelHigh:
			setGPHENN(registerNum, getGPHENN(registerNum) & value);
			break;

		case evLevelLow:
			setGPLENN(registerNum, getGPLENN(registerNum) & value);
			break;
	}
}

void GPIO::setPin(int pinNum)
{
	int				registerNum = 0;
	uint32_t		mask = 0x00000000;
	
	registerNum = _getTwinRegisterNum(pinNum);
	mask = _getTwinRegisterMask(pinNum);
	
	setGPSETN(registerNum, mask);
}

void GPIO::clrPin(int pinNum)
{
	int				registerNum = 0;
	uint32_t		mask = 0x00000000;
	
	registerNum = _getTwinRegisterNum(pinNum);
	mask = _getTwinRegisterMask(pinNum);
	
	setGPCLRN(registerNum, mask);
}

uint32_t GPIO::getGPFSELN(int n) {
	printf("In getGPFSELN()...\n");
	fflush(stdout);
	
	return gpioMap->GPFSEL[n];
}
void GPIO::setGPFSELN(int n, uint32_t val) {
	gpioMap->GPFSEL[n] = val;
}
void GPIO::setGPSETN(int n, uint32_t val) {
	gpioMap->GPSET[n] = val;
}
void GPIO::setGPCLRN(int n, uint32_t val) {
	gpioMap->GPCLR[n] = val;
}
uint32_t GPIO::getGPLEVN(int n) {
	return gpioMap->GPLEV[n];
}
uint32_t GPIO::getGPEDSN(int n) {
	return gpioMap->GPEDS[n];
}
void GPIO::setGPEDSN(int n, uint32_t val) {
	gpioMap->GPEDS[n] = val;
}
uint32_t GPIO::getGPRENN(int n) {
	return gpioMap->GPREN[n];
}
void GPIO::setGPRENN(int n, uint32_t val) {
	gpioMap->GPREN[n] = val;
}
uint32_t GPIO::getGPFENN(int n) {
	return gpioMap->GPFEN[n];
}
void GPIO::setGPFENN(int n, uint32_t val) {
	gpioMap->GPFEN[n] = val;
}
uint32_t GPIO::getGPHENN(int n) {
	return gpioMap->GPHEN[n];
}
void GPIO::setGPHENN(int n, uint32_t val) {
	gpioMap->GPHEN[n] = val;
}
uint32_t GPIO::getGPLENN(int n) {
	return gpioMap->GPLEN[n];
}
void GPIO::setGPLENN(int n, uint32_t val) {
	gpioMap->GPLEN[n] = val;
}
uint32_t GPIO::getGPARENN(int n) {
	return gpioMap->GPAREN[n];
}
void GPIO::setGPARENN(int n, uint32_t val) {
	gpioMap->GPAREN[n] = val;
}
uint32_t GPIO::getGPAFENN(int n) {
	return gpioMap->GPAFEN[n];
}
void GPIO::setGPAFENN(int n, uint32_t val) {
	gpioMap->GPAFEN[n] = val;
}
uint32_t GPIO::getGPPUD() {
	return gpioMap->GPPUD;
}
void GPIO::setGPPUD(uint32_t val) {
	gpioMap->GPPUD = val;
}
uint32_t GPIO::getGPPUDCLKN(int n) {
	return gpioMap->GPPUDCLK[n];
}
void GPIO::setGPPUDCLKN(int n, uint32_t val) {
	gpioMap->GPPUDCLK[n] = val;
}
