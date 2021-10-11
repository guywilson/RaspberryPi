#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pifactory.h"
#include "exception.h"

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
#define RASPPI_PERI_BASE_4		0x7E000000

RaspberryPi * PiFactory::makePi()
{
	int				model;
	RaspberryPi	*	pi;
	
	model = _getModel();
	
	printf("Model is %d\n", model);
	
	switch (model) {
		case PI_MODEL_A:
		case PI_MODEL_B:
		case PI_MODEL_AP:
		case PI_MODEL_BP:
		case PI_ALPHA:
		case PI_MODEL_CM:
		case PI_MODEL_ZERO:
			pi = new RaspberryPiOriginal(RASPPI_PERI_BASE_OLD);
			break ;

		default:
			pi = new RaspberryPiNew(RASPPI_PERI_BASE_NEW);
			break ;
	}
	
	return pi;
}

int PiFactory::_getModel()
{
	FILE *			cpuFd;
	char 			szLine[120];
	char *			c;
	unsigned int	revision;
	unsigned int	lineLength;
	int				model;

	cpuFd = fopen("/proc/cpuinfo", "r");
	
	if (cpuFd == NULL) {
		throw new Exception(
					ERR_FILE_OPEN_FAILURE,
					"Failed to open /proc/cpuinfo",
					__FILE__,
					"PiFactory",
					"_getModel()",
					__LINE__);
	}

	while (fgets(szLine, 120, cpuFd) != NULL) {
		if (strncmp (szLine, "Revision", 8) == 0) {
			break;
		}
	}

	fclose(cpuFd);

	if (strncmp(szLine, "Revision", 8) != 0) {
		throw new Exception(
					ERR_INVALID_FILE_FORMAT,
					"No 'Revision' token found",
					__FILE__,
					"PiFactory",
					"_getModel()",
					__LINE__);
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
		throw new Exception(
					ERR_INVALID_FILE_FORMAT,
					"No ':' found after 'Revision' token",
					__FILE__,
					"PiFactory",
					"_getModel()",
					__LINE__);
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