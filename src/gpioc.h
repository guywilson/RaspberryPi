#ifndef _INCL_GPIOC
#define _INCL_GPIOC

#include <stdint.h>

#define GPIOC_PIN_FUNCTION_INPUT        0
#define GPIOC_PIN_FUNCTION_OUTPUT       1

#define GPIOC_PIN_PULL_DOWN				0x01
#define GPIOC_PIN_PULL_UP				0x02
#define GPIOC_PIN_PULL_OFF				0x00

typedef struct {
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

int gpioc_open();
int gpioc_close();
int gpioc_setPinInput(int pin);
int gpioc_setPinOutput(int pin);
int gpioc_getPinDir(int pin);
int gpioc_setPinOn(int pin);
int gpioc_setPinOff(int pin);
int gpioc_getPinState(int pin);
int gpioc_pullDown(int pin);
int gpioc_pullUp(int pin);
int gpioc_getPullState(int pin);

#endif