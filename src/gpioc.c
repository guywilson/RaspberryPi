#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "gpioc.h"

// Pi model types and version numbers
// Intended for the GPIO program Use at your own risk.
#define	PI_MODEL_A			0
#define	PI_MODEL_B			1
#define	PI_MODEL_AP			2
#define	PI_MODEL_BP			3
#define	PI_MODEL_2			4
#define	PI_ALPHA			5
#define	PI_MODEL_CM			6
#define	PI_MODEL_07			7
#define	PI_MODEL_3			8
#define	PI_MODEL_ZERO		9

#define	RASPPI_PERI_BASE_OLD	0x20000000
#define	RASPPI_PERI_BASE_NEW	0x3F000000
#define GPIO_BASE	  		    0x00200000

#define PAGE_SIZE		        4096
#define BLOCK_SIZE		        4096

#define getTwinRegisterNum(pin)         (pin > 31 ? 1 : 0)
#define getTwinRegisterMask(pin)        (1 << (pin > 31 ? (pin - 32) : pin))

volatile GPIO_MAP * gpioMap;
volatile uint32_t *	map;

int                 memFd;

int _getModel()
{
	FILE *			cpuFd;
	char 			szLine[120];
	char *			c;
	unsigned int	revision;
	unsigned int	lineLength;
	int				model;

	cpuFd = fopen("/proc/cpuinfo", "r");
	
	if (cpuFd == NULL) {
        printf("_getModel: Failed to open /proc/cpuinfo\n");
        return -1;
	}

	while (fgets(szLine, 120, cpuFd) != NULL) {
		if (strncmp (szLine, "Revision", 8) == 0) {
			break;
		}
	}

	fclose(cpuFd);

	if (strncmp(szLine, "Revision", 8) != 0) {
        printf("_getModel: No 'Revision' token found in cpuinfo\n");
        return -1;
	}

	lineLength = strlen(szLine);
	
	// Chomp trailing CR/NL
	for (c = &szLine[lineLength - 1]; (*c == '\n') || (*c == '\r'); --c) {
		*c = 0;
	}
  
	// Need to work out if it's using the new or old encoding scheme:

	// Scan to the first character of the revision number

	for (c = szLine; *c; ++c) {
		if (*c == ':') {
			break;
		}
	}

	if (*c != ':') {
        printf("_getModel: No ':' found after 'Revision' token\n");
        return -1;
	}

	// Chomp spaces
	++c;
	while (isspace(*c)) {
		++c;
	}

	if (!isxdigit(*c)) {
		printf("No hex digit found at beginning of 'Revision' identifier\n");
	}

	revision = (unsigned int)strtol(c, NULL, 16); // Hex number with no leading 0x

	if ((revision & (1 << 23)) != 0)	// New way
	{
		model = (revision & (0xFF <<  4)) >>  4;
	}
	else					// Old way
	{
		if (!isdigit(*c)) {
			printf("Error - no digit at start of revision");
		}

		// Extract last 4 characters:

		c = c + strlen(c) - 4;

		// Fill out the replys as appropriate

		/**/ if (strncmp(c, "0002", 4) == 0) { model = PI_MODEL_B; }
		else if (strncmp(c, "0003", 4) == 0) { model = PI_MODEL_B; }
                                           
		else if (strncmp(c, "0004", 4) == 0) { model = PI_MODEL_B; }
		else if (strncmp(c, "0005", 4) == 0) { model = PI_MODEL_B; }
		else if (strncmp(c, "0006", 4) == 0) { model = PI_MODEL_B; }
                                           
		else if (strncmp(c, "0007", 4) == 0) { model = PI_MODEL_A; }
		else if (strncmp(c, "0008", 4) == 0) { model = PI_MODEL_A; }
		else if (strncmp(c, "0009", 4) == 0) { model = PI_MODEL_A; }
                                           
		else if (strncmp(c, "000d", 4) == 0) { model = PI_MODEL_B; }
		else if (strncmp(c, "000e", 4) == 0) { model = PI_MODEL_B; }
		else if (strncmp(c, "000f", 4) == 0) { model = PI_MODEL_B; }
                                           
		else if (strncmp(c, "0010", 4) == 0) { model = PI_MODEL_BP; }
		else if (strncmp(c, "0013", 4) == 0) { model = PI_MODEL_BP; }
		else if (strncmp(c, "0016", 4) == 0) { model = PI_MODEL_BP; }
		else if (strncmp(c, "0019", 4) == 0) { model = PI_MODEL_BP; }
                                           
		else if (strncmp(c, "0011", 4) == 0) { model = PI_MODEL_CM; }
		else if (strncmp(c, "0014", 4) == 0) { model = PI_MODEL_CM; }
		else if (strncmp(c, "0017", 4) == 0) { model = PI_MODEL_CM; }
		else if (strncmp(c, "001a", 4) == 0) { model = PI_MODEL_CM; }
                                           
		else if (strncmp(c, "0012", 4) == 0) { model = PI_MODEL_AP; }
		else if (strncmp(c, "0015", 4) == 0) { model = PI_MODEL_AP; }
		else if (strncmp(c, "0018", 4) == 0) { model = PI_MODEL_AP; }
		else if (strncmp(c, "001b", 4) == 0) { model = PI_MODEL_AP; }
                                            
		else                              { model = 0;  }
	}
	
	return model;
}

uint32_t getGPFSELN(int n) {
	return gpioMap->GPFSEL[n];
}
void setGPFSELN(int n, uint32_t val) {
	gpioMap->GPFSEL[n] = val;
}
void setGPSETN(int n, uint32_t val) {
	gpioMap->GPSET[n] = val;
}
void setGPCLRN(int n, uint32_t val) {
	gpioMap->GPCLR[n] = val;
}
uint32_t getGPLEVN(int n) {
	return gpioMap->GPLEV[n];
}
uint32_t getGPEDSN(int n) {
	return gpioMap->GPEDS[n];
}
void setGPEDSN(int n, uint32_t val) {
	gpioMap->GPEDS[n] = val;
}
uint32_t getGPRENN(int n) {
	return gpioMap->GPREN[n];
}
void setGPRENN(int n, uint32_t val) {
	gpioMap->GPREN[n] = val;
}
uint32_t getGPFENN(int n) {
	return gpioMap->GPFEN[n];
}
void setGPFENN(int n, uint32_t val) {
	gpioMap->GPFEN[n] = val;
}
uint32_t getGPHENN(int n) {
	return gpioMap->GPHEN[n];
}
void setGPHENN(int n, uint32_t val) {
	gpioMap->GPHEN[n] = val;
}
uint32_t getGPLENN(int n) {
	return gpioMap->GPLEN[n];
}
void setGPLENN(int n, uint32_t val) {
	gpioMap->GPLEN[n] = val;
}
uint32_t getGPARENN(int n) {
	return gpioMap->GPAREN[n];
}
void setGPARENN(int n, uint32_t val) {
	gpioMap->GPAREN[n] = val;
}
uint32_t getGPAFENN(int n) {
	return gpioMap->GPAFEN[n];
}
void setGPAFENN(int n, uint32_t val) {
	gpioMap->GPAFEN[n] = val;
}
uint32_t getGPPUD() {
	return gpioMap->GPPUD;
}
void setGPPUD(uint32_t val) {
	gpioMap->GPPUD = val;
}
uint32_t getGPPUDCLKN(int n) {
	return gpioMap->GPPUDCLK[n];
}
void setGPPUDCLKN(int n, uint32_t val) {
	gpioMap->GPPUDCLK[n] = val;
}

int gpioc_open()
{
	int				    model;
    volatile uint32_t	pPeripheralBase;

    /*
    ** Check privileges...
    */
	if (geteuid() != 0) {
        printf("gpioc_open: Unable to access protected memory, must run as root user\n");
        return -1;
	}

	model = _getModel();

    memFd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
    
    if (memFd < 0) {
        memFd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC);
    }
    
    if (memFd < 0) {
        printf("gpioc_open: Failed to open /dev/gpiomem\n");
        return -1;
    }
	
	switch (model) {
		case PI_MODEL_A:
		case PI_MODEL_B:
		case PI_MODEL_AP:
		case PI_MODEL_BP:
		case PI_ALPHA:
		case PI_MODEL_CM:
		case PI_MODEL_ZERO:
            pPeripheralBase = RASPPI_PERI_BASE_OLD;
            break ;

		default:
            pPeripheralBase = RASPPI_PERI_BASE_NEW;
			break ;
	}

	map = (uint32_t *)mmap(
						0,
						BLOCK_SIZE,
						PROT_READ | PROT_WRITE,
						MAP_SHARED,
						memFd,
						pPeripheralBase + GPIO_BASE);

	if (map < 0L) {
        printf("gpioc_open: Failed to map GPIO memory\n");
        return -1;
	}

	gpioMap = (GPIO_MAP *)map;

    return 0;
}

int gpioc_close()
{
	munmap((uint32_t *)map, BLOCK_SIZE);
    return close(memFd);
}

int gpioc_setPinInput(int pin)
{
	int				registerNum = 0;
	int				shift = 0;
	uint32_t		newMask = 0x00000000;
	uint32_t		currentMask = 0x00000000;
	
	registerNum = pin / 10;
	shift = (pin % 10) * 3;
	
	currentMask = getGPFSELN(registerNum);

	newMask = currentMask & (~(7 << shift));
	
	setGPFSELN(registerNum, newMask);

    return 0;
}

int gpioc_setPinOutput(int pin)
{
	int				registerNum = 0;
	int				shift = 0;
	uint32_t		newMask = 0x00000000;
	uint32_t		currentMask = 0x00000000;
	
	registerNum = pin / 10;
	shift = (pin % 10) * 3;
	
	currentMask = getGPFSELN(registerNum);

	newMask = (currentMask & (~(7 << shift))) | (0x00000001 << shift);
	
	setGPFSELN(registerNum, newMask);

    return 0;
}

int gpioc_getPinDir(int pin)
{
	int				registerNum = 0;
	int				shift = 0;
	uint32_t		currentMask = 0x00000000;
	int		        function;
	
	registerNum = pin / 10;
	shift = (pin % 10) * 3;
	
	currentMask = getGPFSELN(registerNum);
	
	function = (int)(((currentMask & (7 << shift)) >> shift) & 0xFF);
	
	return function;
}

int gpioc_setPinOn(int pin)
{
	int				registerNum = 0;
	uint32_t		mask = 0x00000000;
	
	registerNum = getTwinRegisterNum(pin);
	mask = getTwinRegisterMask(pin);
	
	setGPSETN(registerNum, mask);

    return 0;
}

int gpioc_setPinOff(int pin)
{
	int				registerNum = 0;
	uint32_t		mask = 0x00000000;
	
	registerNum = getTwinRegisterNum(pin);
	mask = getTwinRegisterMask(pin);
	
	setGPCLRN(registerNum, mask);

    return 0;
}

int gpioc_getPinState(int pin)
{
	int				registerNum = 0;
	uint32_t		reg = 0x00000000;
	uint32_t		mask = 0x00000000;
	int			    level = 0;
	
	registerNum = getTwinRegisterNum(pin);
	mask = getTwinRegisterMask(pin);

	reg = getGPLEVN(registerNum);
	reg &= mask;
	
	level = reg > 0 ? 1 : 0;
	
	return level;
}
