#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "spi.h"
#include "register.h"
#include "swtimer.h"

#define SPI_BASE				0x00204000

#define CORE_CLK_SPEED			250000000L


volatile SPI_MAP * spiMap;

SPI::SPI(int memFd, uint32_t baseAddress) : ProtectedMemory_Peripheral(memFd, baseAddress + SPI_BASE)
{
	spiMap = (SPI_MAP *)getMemoryMap();
}

void SPI::setup(uint32_t clkFrequency, uint32_t flags)
{
	uint32_t		clkDivisor;
	uint32_t		ctrl;
	
	/*
	** Protect from divide by zero...
	*/
	if (clkFrequency == 0) {
		clkFrequency = 1;
	}
	
	clkDivisor = (CORE_CLK_SPEED / clkFrequency);
	
	setSPI_CLK(clkDivisor);
	
	ctrl = getSPI_CS();
	
	ctrl &= ~SPI_CTRL_MASK;
	ctrl |= (flags & SPI_CTRL_MASK);
	
	setSPI_CS(ctrl);
}

void SPI::select(SpiChip chipId)
{
	uint32_t		ctrl;
	
	ctrl = getSPI_CS();
	
	ctrl &= 0xFFFFFFFC;
	ctrl |= (uint32_t)(chipId & 0x03);
	
	setSPI_CS(ctrl);
}

void SPI::enable()
{
	uint32_t		ctrl;
	
	ctrl = getSPI_CS();
	
	ctrl |= SPI_CTRL_TRANSFER_ACTIVE;
	
	setSPI_CS(ctrl);
}

void SPI::disable()
{
	uint32_t		ctrl;
	
	ctrl = getSPI_CS();
	
	ctrl &= ~SPI_CTRL_TRANSFER_ACTIVE;
	
	setSPI_CS(ctrl);
}

void SPI::clearTx()
{
	uint32_t		ctrl;
	
	ctrl = getSPI_CS();
	
	ctrl |= SPI_CTRL_CLEARFIFO_TX;
	
	setSPI_CS(ctrl);
}

void SPI::clearRx()
{
	uint32_t		ctrl;
	
	ctrl = getSPI_CS();
	
	ctrl |= SPI_CTRL_CLEARFIFO_RX;
	
	setSPI_CS(ctrl);
}

uint32_t SPI::getStatus()
{
	return getSPI_CS() & SPI_STATUS_MASK;
}

bool SPI::isDone()
{
	return((getSPI_CS() & SPI_STATUS_TX_DONE) > 0 ? true : false);
}

bool SPI::isActive()
{
	return((getSPI_CS() & SPI_STATUS_TX_ACTIVE) > 0 ? true : false);
}

bool SPI::isRxDataAvailable()
{
	return((getSPI_CS() & SPI_STATUS_RX_DATA_AVAILABLE) > 0 ? true : false);
}

bool SPI::isRxFull()
{
	return((getSPI_CS() & SPI_STATUS_RX_FIFO_FULL) > 0 ? true : false);
}

bool SPI::isRxNearlyFull()
{
	return((getSPI_CS() & SPI_STATUS_RX_FIFO_NEARLY_FULL) > 0 ? true : false);
}

bool SPI::isTxSpaceAvailable()
{
	return((getSPI_CS() & SPI_STATUS_TX_SPACE_AVAILABLE) > 0 ? true : false);
}


uint32_t SPI::getSPI_CS() {
	return spiMap->SPI_CS & 0x03FFFFFF;
}
void SPI::setSPI_CS(uint32_t value) {
	spiMap->SPI_CS = value & SPI_CTRL_MASK;
}
uint32_t SPI::getSPI_FIFO() {
	return spiMap->SPI_FIFO;
}
void SPI::setSPI_FIFO(uint32_t value) {
	spiMap->SPI_FIFO = value;
}
uint32_t SPI::getSPI_CLK() {
	return spiMap->SPI_CLK & 0x0000FFFF;
}
void SPI::setSPI_CLK(uint32_t value) {
	spiMap->SPI_CLK = value & 0x0000FFFF;
}
uint32_t SPI::getSPI_DLEN() {
	return spiMap->SPI_DLEN;
}
void SPI::setSPI_DLEN(uint32_t value) {
	spiMap->SPI_DLEN = value;
}
uint32_t SPI::getSPI_LTOH() {
	return spiMap->SPI_LTOH;
}
void SPI::setSPI_LTOH(uint32_t value) {
	spiMap->SPI_LTOH = value;
}
uint32_t SPI::getSPI_DC() {
	return spiMap->SPI_DC;
}
void SPI::setSPI_DC(uint32_t value) {
	spiMap->SPI_DC = value;
}
