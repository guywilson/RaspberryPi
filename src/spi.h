
#ifndef _INC_SPI
#define _INC_SPI

#include "peripheral.h"

/*
**   Bit mask to Hex helper
**
**   22 2222 1111 1111 1100 0000 0000			- Bit
**   54 3210 9876 5432 1098 7654 3210			- Number
**
**   21 8421 8421 8421 8421 8421 8421
**
**   11 1110 0000 0011 1111 1111 1111
**  0x3    E    0    3    F    F    F
**  0x0    1    F    C    0    0    0
*/

/*
** CTRL flags
*/
#define SPI_CTRL_CLOCKPHASE_MIDDLE			0x00000000
#define SPI_CTRL_CLOCKPHASE_BEGINNING		0x00000004

#define SPI_CTRL_CLOCKPOLARITY_LOW			0x00000000
#define SPI_CTRL_CLOCKPOLARITY_HIGH			0x00000008

#define SPI_CTRL_CLEARFIFO_TX				0x00000010
#define SPI_CTRL_CLEARFIFO_RX				0x00000020

#define SPI_CTRL_CSPOLARITY_LOW				0x00000000
#define SPI_CTRL_CSPOLARITY_HIGH			0x00000040

#define SPI_CTRL_TRANSFER_INACTIVE			0x00000000
#define SPI_CTRL_TRANSFER_ACTIVE			0x00000080

#define SPI_CTRL_DMA_DISABLE				0x00000000
#define SPI_CTRL_DMA_ENABLE					0x00000100

#define SPI_CTRL_INTTXDONE_DISABLE			0x00000000
#define SPI_CTRL_INTTXDONE_ENABLE			0x00000200

#define SPI_CTRL_INTRXREAD_DISABLE			0x00000000
#define SPI_CTRL_INTRXREAD_ENABLE			0x00000400

#define SPI_CTRL_AUTODROPDMACS_DISABLE		0x00000000
#define SPI_CTRL_AUTODROPDMACS_ENABLE		0x00000800

#define SPI_CTRL_BIDIRECTIONALMODE_WRITE	0x00000000
#define SPI_CTRL_BIDIRECTIONALMODE_READ		0x00001000

#define SPI_CTRL_LOSSIMODE_DISABLE			0x00000000
#define SPI_CTRL_LOSSIMODE_ENABLE			0x00002000

#define SPI_CTRL_CSPOLARITY_0_ACTIVELOW		0x00000000
#define SPI_CTRL_CSPOLARITY_0_ACTIVEHIGH	0x00200000

#define SPI_CTRL_CSPOLARITY_1_ACTIVELOW		0x00000000
#define SPI_CTRL_CSPOLARITY_1_ACTIVEHIGH	0x00400000

#define SPI_CTRL_CSPOLARITY_2_ACTIVELOW		0x00000000
#define SPI_CTRL_CSPOLARITY_2_ACTIVEHIGH	0x00800000

#define SPI_CTRL_LOSSIMODE_DMA_DISABLE		0x00000000
#define SPI_CTRL_LOSSIMODE_DMA_ENABLE		0x01000000

#define SPI_CTRL_LOSSIMODE_DATALEN_BYTE		0x00000000
#define SPI_CTRL_LOSSIMODE_DATALEN_DWORD	0x02000000

#define SPI_CTRL_MASK						0x03E0EFFF

/*
** STATUS flags
*/
#define SPI_STATUS_TX_ACTIVE				0x00000080
#define SPI_STATUS_TX_DONE					0x00010000
#define SPI_STATUS_RX_DATA_AVAILABLE		0x00020000
#define SPI_STATUS_TX_SPACE_AVAILABLE		0x00040000
#define SPI_STATUS_RX_FIFO_NEARLY_FULL		0x00080000
#define SPI_STATUS_RX_FIFO_FULL				0x00100000

#define SPI_STATUS_MASK						0x001FC000

typedef enum
{
	chip0		= 0x00,
	chip1		= 0x01,
	chip2		= 0x02
}
SpiChip;

class SPI : public ProtectedMemory_Peripheral
{
	public:
		SPI(int memFd, uint32_t baseAddress);
		
		void			setup(uint32_t clkFrequency, uint32_t flags);
		
		void			select(SpiChip chipId);
		
		void			enable();
		void			disable();
		
		void			clearTx();
		void			clearRx();
		
		uint32_t		getStatus();
		
		bool			isDone();
		
		bool			isActive();
		
		bool			isRxDataAvailable();
		bool			isRxFull();
		bool			isRxNearlyFull();

		bool			isTxSpaceAvailable();
		
		
		uint32_t		getSPI_CS();
		void			setSPI_CS(uint32_t value);
		
		uint32_t		getSPI_FIFO();
		void			setSPI_FIFO(uint32_t value);
		
		uint32_t		getSPI_CLK();
		void			setSPI_CLK(uint32_t value);
		
		uint32_t		getSPI_DLEN();
		void			setSPI_DLEN(uint32_t value);
		
		uint32_t		getSPI_LTOH();
		void			setSPI_LTOH(uint32_t value);
		
		uint32_t		getSPI_DC();
		void			setSPI_DC(uint32_t value);
};

#endif
