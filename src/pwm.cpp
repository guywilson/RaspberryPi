#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "clock.h"
#include "register.h"
#include "pwm.h"
#include "swtimer.h"

#define PWM_BASE				0x0020C000

volatile PWM_MAP * pwmMap;

PWM::PWM(int memFd, uint32_t baseAddress) : ProtectedMemory_Peripheral(memFd, baseAddress + PWM_BASE)
{
	pwmMap = (PWM_MAP *)getMemoryMap();
}

CLOCK * PWM::getClock()
{
	return this->pClock;
}

void PWM::setClock(CLOCK * pClock)
{
	this->pClock = pClock;
}

/*
** Disable the whole PWM, useful for wehn setting the clock frequency...
*/
void PWM::disable()
{
	setPWM_CTRL(0x00000000);
	SWTimer::usDelay(150);
}

void PWM::disable(int channel)
{
	setControl(channel, PWM_CTRL_DISABLE);
}

void PWM::enable(int channel)
{
	uint32_t	ctrl;
	
	ctrl = getPWM_CTRL();
	
	switch (channel) {
		case PWM_CHANNEL_0:
			ctrl |= PWM_CTRL_ENABLE;
			break;
			
		case PWM_CHANNEL_1:
			ctrl |= (PWM_CTRL_ENABLE << 8);
			break;
	}

	setPWM_CTRL(ctrl);
	SWTimer::usDelay(150);
}

void PWM::setControl(int channel, uint32_t flags)
{
	uint32_t	ctrl;
	
	ctrl = getPWM_CTRL();
	
	switch (channel) {
		case PWM_CHANNEL_0:
			ctrl &= 0xFF00;
			ctrl |= (flags & 0xFE);
			break;
			
		case PWM_CHANNEL_1:
			ctrl &= 0x00FF;
			ctrl |= ((flags & 0xFE) << 8);
			break;
	}

	setPWM_CTRL(ctrl);
	SWTimer::usDelay(150);
}

void PWM::setup(int channel, uint32_t flags, uint32_t range)
{
	// Set the range (default is 32)...
	setRange(PWM_CHANNEL_0, range);

	setControl(channel, flags);
}

void PWM::setup(int channel, uint32_t flags, unsigned long clkFrequency, uint32_t range)
{
	// Set the PWM clock frequency...
	getClock()->stop(clkPWM);
	getClock()->setFrequency(clkPWM, clkFrequency);
	getClock()->start(clkPWM);
	
	setup(channel, flags, range);
}

uint32_t PWM::getStatus(int channel)
{
	uint32_t		status;
	uint32_t		mask;

	// Generic flags
	mask =	PWM_STATUS_FIFO_FULL | 
			PWM_STATUS_FIFO_EMPTY | 
			PWM_ERROR_FIFO_WRITE | 
			PWM_ERROR_FIFO_READ | 
			PWM_ERROR_BUS;
	
	/*
	** Add in channel specifc flags...
	*/
	switch (channel) {
		case PWM_CHANNEL_0:
			mask |= PWM_ERROR_CH0_GAP | PWM_STATUS_CH0_TX;
			break;
			
		case PWM_CHANNEL_1:
			mask |= PWM_ERROR_CH1_GAP | PWM_STATUS_CH1_TX;
			break;
	}
	
	status = getPWM_STATUS();
	
	return status & mask;
}

bool PWM::isFifoFull()
{
	bool			b;
	
	b = (getPWM_STATUS() & PWM_STATUS_FIFO_FULL) > 0 ? true : false;
	
	return b;
}

bool PWM::isFifoEmpty()
{
	bool			b;
	
	b = (getPWM_STATUS() & PWM_STATUS_FIFO_EMPTY) > 0 ? true : false;
	
	return b;
}

bool PWM::isError(int channel)
{
	bool			b;
	uint32_t		mask;

	// Generic flags
	mask =	PWM_ERROR_FIFO_WRITE | 
			PWM_ERROR_FIFO_READ | 
			PWM_ERROR_BUS;
	
	/*
	** Add in channel specifc flags...
	*/
	switch (channel) {
		case PWM_CHANNEL_0:
			mask |= PWM_ERROR_CH0_GAP;
			break;
			
		case PWM_CHANNEL_1:
			mask |= PWM_ERROR_CH1_GAP;
			break;
	}
	
	b = (getPWM_STATUS() & mask) > 0 ? true : false;
	
	return b;
}

bool PWM::isActive(int channel)
{
	bool			b;
	uint32_t		mask;
	
	switch (channel) {
		case PWM_CHANNEL_0:
			mask = PWM_STATUS_CH0_TX;
			break;
			
		case PWM_CHANNEL_1:
			mask = PWM_STATUS_CH1_TX;
			break;
	}
	
	b = (getPWM_STATUS() & mask) > 0 ? true : false;
	
	return b;
}

void PWM::setRange(int channel, uint32_t range)
{
	switch (channel) {
		case PWM_CHANNEL_0:
			setPWM_RANGE0(range);
			break;
			
		case PWM_CHANNEL_1:
			setPWM_RANGE1(range);
			break;
	}
	
	usleep(150);
}

void PWM::setData(int channel, uint32_t data)
{
	switch (channel) {
		case PWM_CHANNEL_0:
			setPWM_DATA0(data);
			break;
			
		case PWM_CHANNEL_1:
			setPWM_DATA1(data);
			break;
	}
}


uint32_t PWM::getPWM_CTRL() {
	return pwmMap->PWM_CTRL & 0x0000FFFF;
}
void PWM::setPWM_CTRL(uint32_t value) {
	pwmMap->PWM_CTRL = value & 0x0000FFFF;
}
uint32_t PWM::getPWM_STATUS() {
	return pwmMap->PWM_STATUS & 0x00001FFF;
}
void PWM::setPWM_STATUS(uint32_t value) {
	pwmMap->PWM_STATUS = value & 0x00001FFF;
}
uint32_t PWM::getPWM_DMAC() {
	return pwmMap->PWM_DMAC;
}
void PWM::setPWM_DMAC(uint32_t value) {
	pwmMap->PWM_DMAC = value;
}
uint32_t PWM::getPWM_RANGE0() {
	return pwmMap->PWM_RANGE0;
}
void PWM::setPWM_RANGE0(uint32_t value) {
	pwmMap->PWM_RANGE0 = value;
}
uint32_t PWM::getPWM_DATA0() {
	return pwmMap->PWM_DATA0;
}
void PWM::setPWM_DATA0(uint32_t value) {
	pwmMap->PWM_DATA0 = value;
}
uint32_t PWM::getPWM_FIFO() {
	return pwmMap->PWM_FIFO;
}
void PWM::setPWM_FIFO(uint32_t value) {
	pwmMap->PWM_FIFO = value;
}
uint32_t PWM::getPWM_RANGE1() {
	return pwmMap->PWM_RANGE1;
}
void PWM::setPWM_RANGE1(uint32_t value) {
	pwmMap->PWM_RANGE1 = value;
}
uint32_t PWM::getPWM_DATA1() {
	return pwmMap->PWM_DATA1;
}
void PWM::setPWM_DATA1(uint32_t value) {
	pwmMap->PWM_DATA1 = value;
}
