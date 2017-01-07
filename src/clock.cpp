#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "register.h"
#include "clock.h"
#include "exception.h"
#include "swtimer.h"

#define GPIO_CLOCK_BASE			0x00101000
#define CLOCK_MGR_PASSWORD		0x5A000000
#define CLOCK_SRC_FREQ			19200000L

volatile CLOCK_MAP * clockMap;


CLOCK::CLOCK(int memFd, uint32_t baseAddress) : ProtectedMemory_Peripheral(memFd, baseAddress + GPIO_CLOCK_BASE)
{
	clockMap = (CLOCK_MAP *)getMemoryMap();
}

bool CLOCK::isBusy(ClockId id)
{
	bool		isBusy;
	uint32_t	ctrlRegister;
	
	switch (id) {
		case clk0:
			ctrlRegister = getCLKMGR_0();
			break;

		case clk2:
			ctrlRegister = getCLKMGR_2();
			break;

		case clkPWM:
			ctrlRegister = getCLKMGR_PWM();
			break;
		
		case clk1:
			throw new Exception(
						ERR_PERIPHERAL_IN_USE,
						"Cannot access CLOCK1 on the Raspberry Pi, CLOCK1 is in use",
						__FILE__,
						"CLOCK",
						"isBusy()",
						__LINE__);
	}

	isBusy = (ctrlRegister & 0x00000080) > 0 ? true : false;
	
	return isBusy;
}

void CLOCK::waitWhileBusy(ClockId id)
{
	while (isBusy(id)) {
		SWTimer::usDelay(30);
	}
}

void CLOCK::start(ClockId id)
{
	uint32_t	ctrlRegister;
	
	// Stop the clock if it is already running...
	if (isBusy(id)) {
		stop(id);
		
		waitWhileBusy(id);
	}
	
	ctrlRegister = 0x00000011;
	
	switch (id) {
		case clk0:
			setCLKMGR_0(ctrlRegister);
			break;

		case clk2:
			setCLKMGR_2(ctrlRegister);
			break;

		case clkPWM:
			setCLKMGR_PWM(ctrlRegister);
			break;
		
		case clk1:
			throw new Exception(
						ERR_PERIPHERAL_IN_USE,
						"Cannot access CLOCK1 on the Raspberry Pi, CLOCK1 is in use",
						__FILE__,
						"CLOCK",
						"start()",
						__LINE__);
	}
}

void CLOCK::stop(ClockId id)
{
	uint32_t	ctrlRegister;
	
	ctrlRegister = 0x00000001;
	
	switch (id) {
		case clk0:
			setCLKMGR_0(ctrlRegister);
			break;

		case clk2:
			setCLKMGR_2(ctrlRegister);
			break;

		case clkPWM:
			setCLKMGR_PWM(ctrlRegister);
			break;
		
		case clk1:
			throw new Exception(
						ERR_PERIPHERAL_IN_USE,
						"Cannot access CLOCK1 on the Raspberry Pi, CLOCK1 is in use",
						__FILE__,
						"CLOCK",
						"stop()",
						__LINE__);
	}
	
	SWTimer::usDelay(150);
}

unsigned long CLOCK::getFrequency(ClockId id)
{
	uint32_t			divisor;
	unsigned long		freq;
	
	switch (id) {
		case clk0:
			divisor = getCLKDIV_0();
			break;

		case clk2:
			divisor = getCLKDIV_2();
			break;

		case clkPWM:
			divisor = getCLKDIV_PWM();
			break;
		
		case clk1:
			throw new Exception(
						ERR_PERIPHERAL_IN_USE,
						"Cannot access CLOCK1 on the Raspberry Pi, CLOCK1 is in use",
						__FILE__,
						"CLOCK",
						"getFrequency()",
						__LINE__);
	}
	
	freq = CLOCK_SRC_FREQ / (divisor & 0x00FFF000);
	
	return freq;
}

void CLOCK::setFrequency(ClockId id, unsigned long frequency)
{
	uint32_t			divisor;
	
	// Stop the clock if it is already running...
	if (isBusy(id)) {
		stop(id);
		
		waitWhileBusy(id);
	}
	
	divisor = ((CLOCK_SRC_FREQ / frequency) << 12) & 0x00FFF000;
	
	switch (id) {
		case clk0:
			setCLKDIV_0(divisor);
			break;

		case clk2:
			setCLKDIV_2(divisor);
			break;

		case clkPWM:
			setCLKDIV_PWM(divisor);
			break;
		
		case clk1:
			throw new Exception(
						ERR_PERIPHERAL_IN_USE,
						"Cannot access CLOCK1 on the Raspberry Pi, CLOCK1 is in use",
						__FILE__,
						"CLOCK",
						"setFrequency()",
						__LINE__);
	}
}

		
uint32_t CLOCK::getCLKMGR_0() {
	return clockMap->CLKMGR_0 & 0x00FFFFFF;
}
void CLOCK::setCLKMGR_0(uint32_t value) {
	clockMap->CLKMGR_0 = CLOCK_MGR_PASSWORD | value;
}
uint32_t CLOCK::getCLKDIV_0() {
	return clockMap->CLKDIV_0 & 0x00FFFFFF;
}
void CLOCK::setCLKDIV_0(uint32_t value) {
	clockMap->CLKDIV_0 = CLOCK_MGR_PASSWORD | value;
}

/*
** Clock 1 reserved for system use...
** 
uint32_t CLOCK::getCLKMGR_1() {
	return clockMap->CLKMGR_1 & 0x00FFFFFF;
}
void CLOCK::setCLKMGR_1(uint32_t value) {
	clockMap->CLKMGR_1 = CLOCK_MGR_PASSWORD | value;
}
uint32_t CLOCK::getCLKDIV_1() {
	return clockMap->CLKDIV_1 & 0x00FFFFFF;
}
void CLOCK::setCLKDIV_1(uint32_t value) {
	clockMap->CLKDIV_1 = CLOCK_MGR_PASSWORD | value;
}
*/

uint32_t CLOCK::getCLKMGR_2() {
	return clockMap->CLKMGR_2 & 0x00FFFFFF;
}
void CLOCK::setCLKMGR_2(uint32_t value) {
	clockMap->CLKMGR_2 = CLOCK_MGR_PASSWORD | value;
}
uint32_t CLOCK::getCLKDIV_2() {
	return clockMap->CLKDIV_2 & 0x00FFFFFF;
}
void CLOCK::setCLKDIV_2(uint32_t value) {
	clockMap->CLKDIV_2 = CLOCK_MGR_PASSWORD | value;
}
uint32_t CLOCK::getCLKMGR_PWM() {
	return clockMap->CLKMGR_PWM & 0x00FFFFFF;
}
void CLOCK::setCLKMGR_PWM(uint32_t value) {
	clockMap->CLKMGR_PWM = CLOCK_MGR_PASSWORD | value;
}
uint32_t CLOCK::getCLKDIV_PWM() {
	return clockMap->CLKDIV_PWM & 0x00FFFFFF;
}
void CLOCK::setCLKDIV_PWM(uint32_t value) {
	clockMap->CLKDIV_PWM = CLOCK_MGR_PASSWORD | value;
}
