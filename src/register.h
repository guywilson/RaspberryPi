
#ifndef _INC_REGISTER
#define _INC_REGISTER

#include <stdint.h>


typedef struct
{

//	Type			Name			Description								Size	R/W
//-------------------------------------------------------------------------------------
	uint32_t		GPFSEL[6];	//	GPIO Function Select 0 - 5				32 x 6	R/W
	uint32_t		_RSRVD0;	//	Reserved 								32 		-
	uint32_t		GPSET[2]; 	//	GPIO Pin Output Set 0, 1				32 x 2	W
	uint32_t		_RSRVD1;	//	Reserved 								32 		-
	uint32_t		GPCLR[2]; 	//	GPIO Pin Output Clear 0, 1 				32 x 2	W
	uint32_t		_RSRVD2;	//	Reserved 								32 		-
	uint32_t		GPLEV[2];	//	GPIO Pin Level 0, 1						32 x 2	R
	uint32_t		_RSRVD3;	//	Reserved 								32 		-
	uint32_t		GPEDS[2]; 	//	GPIO Pin Event Detect Status 0, 1		32 x 2	R/W
	uint32_t		_RSRVD4;	//	Reserved 								32 		-
	uint32_t		GPREN[2]; 	//	GPIO Pin Rising Edge Detect Enable 0, 1	32 x 2	R/W
	uint32_t		_RSRVD5;	//	Reserved 								32 		-
	uint32_t		GPFEN[2]; 	//	GPIO Pin Falling Edge Detect Enable 0,1 32 x 2	R/W
	uint32_t		_RSRVD6;	//	Reserved 								32 		-
	uint32_t		GPHEN[2]; 	//	GPIO Pin High Detect Enable 0, 1		32 x 2	R/W
	uint32_t		_RSRVD7;	//	Reserved 								32 		-
	uint32_t		GPLEN[2]; 	//	GPIO Pin Low Detect Enable 0, 1			32 x 2	R/W
	uint32_t		_RSRVD8;	//	Reserved 								32 		-
	uint32_t		GPAREN[2]; 	//	GPIO Pin Async. Rising Edge Detect 0, 1	32 x 2	R/W
	uint32_t		_RSRVD9;	//	Reserved 								32 		-
	uint32_t		GPAFEN[2]; 	//	GPIO Pin Async. Falling Edge Detect 0,1 32 x 2	R/W
	uint32_t		_RSRVD10;	//	Reserved 								32 		-
	uint32_t		GPPUD; 		//	GPIO Pin Pull-up/down Enable 			32 		R/W
	uint32_t		GPPUDCLK[2];//	GPIO Pin Pull-up/down Enable Clock 0, 1	32 x 2	R/W
}
GPIO_MAP;

typedef struct
{
	uint32_t		_RSRVD0[28];
	
	uint32_t		CLKMGR_0;		// 0x00101070
	uint32_t		CLKDIV_0;		// 0x00101074
	
	uint32_t		CLKMGR_1;		// 0x00101078
	uint32_t		CLKDIV_1;		// 0x0010107C

	uint32_t		CLKMGR_2;		// 0x00101080
	uint32_t		CLKDIV_2;		// 0x00101084
	
	uint32_t		_RSRVD1[6];
	
	uint32_t		CLKMGR_PWM;		// 0x001010A0
	uint32_t		CLKDIV_PWM;		// 0x001010A4
}
CLOCK_MAP;

typedef struct
{
	
//					Register Name		Description					Size
//----------------------------------------------------------------------
	uint32_t		PWM_CTRL;		//	PWM Control Register		32
	uint32_t		PWM_STATUS;		//	PWM Status Register			32
	uint32_t		PWM_DMAC;		//	PWM DMA Configuration		32
	
	uint32_t		_RSRVD0;
	
	uint32_t		PWM_RANGE0;		//	PWM Channel 0 Range			32
	uint32_t		PWM_DATA0;		//	PWM Channel 0 Tx Data		32
	
	uint32_t		PWM_FIFO;		//	PWM FIFO Tx Register		32
	
	uint32_t		_RSRVD1;
	
	uint32_t		PWM_RANGE1;		//	PWM Channel 1 Range			32
	uint32_t		PWM_DATA1;		//	PWM Channel 1 Tx Data		32
}
PWM_MAP;

typedef struct
{
	
//					Register Name		Description					Size
//----------------------------------------------------------------------
	uint32_t		SPI_CS;			//	SPI Control & Status		32
	uint32_t		SPI_FIFO;		//	SPI Tx & Rx FIFOs			32
	uint32_t		SPI_CLK;		//	SPI Clock Divider			32
	uint32_t		SPI_DLEN;		//	SPI Data Length				32
	uint32_t		SPI_LTOH;		//	SPI LOSSI Mode TOH			32
	uint32_t		SPI_DC;			//	SPI DMA DREQ Control		32
}
SPI_MAP;

#endif
