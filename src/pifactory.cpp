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
	uint32_t						revisionId;
	RaspberryPi	*					pi;
	RaspberryPi::Manufacturer		manufacturer;
	RaspberryPi::MemorySize			memSize;
	RaspberryPi::Model				model;
	RaspberryPi::Processor			proc;
	const char *					pszRevision;
	uint32_t						revisionIdCopy;
	char 							szRevision[8];

	revisionId = _getRevision();
	
	bool isNew = (revisionId & (1 << 23)) != 0 ? true : false;

	if (!isNew) {
		revisionId = revisionId & 0xFFFF;

		/************************************************
		**
		** Old Revision Codes
		**
		*************************************************

		Code	Model	Revision	RAM		Manufacturer
		----	-----	--------	-----	------------
		0002	B		1.0			256MB	Egoman
		0003	B		1.0			256MB	Egoman
		0004	B		2.0			256MB	Sony UK
		0005	B		2.0			256MB	Qisda
		0006	B		2.0			256MB	Egoman
		0007	A		2.0			256MB	Egoman
		0008	A		2.0			256MB	Sony UK
		0009	A		2.0			256MB	Qisda
		000d	B		2.0			512MB	Egoman
		000e	B		2.0			512MB	Sony UK
		000f	B		2.0			512MB	Egoman
		0010	B+		1.2			512MB	Sony UK
		0011	CM1		1.0			512MB	Sony UK
		0012	A+		1.1			256MB	Sony UK
		0013	B+		1.2			512MB	Embest
		0014	CM1		1.0			512MB	Embest
		0015	A+		1.1			512MB	Embest
		************************************************/

		switch (revisionId) {
			case 0x0002:
			case 0x0003:
				manufacturer = RaspberryPi::Manufacturer::Egoman;
				memSize = RaspberryPi::MemorySize::_256Mb;
				model = RaspberryPi::Model::Rpi_Model_B;
				pszRevision = "1.0";
				break;

			case 0x0004:
				manufacturer = RaspberryPi::Manufacturer::SonyUK;
				memSize = RaspberryPi::MemorySize::_256Mb;
				model = RaspberryPi::Model::Rpi_Model_B;
				pszRevision = "2.0";
				break;

			case 0x0005:
				manufacturer = RaspberryPi::Manufacturer::Qisda;
				memSize = RaspberryPi::MemorySize::_256Mb;
				model = RaspberryPi::Model::Rpi_Model_B;
				pszRevision = "2.0";
				break;

			case 0x0006:
				manufacturer = RaspberryPi::Manufacturer::Egoman;
				memSize = RaspberryPi::MemorySize::_256Mb;
				model = RaspberryPi::Model::Rpi_Model_B;
				pszRevision = "2.0";
				break;

			case 0x0007:
				manufacturer = RaspberryPi::Manufacturer::Egoman;
				memSize = RaspberryPi::MemorySize::_256Mb;
				model = RaspberryPi::Model::Rpi_Model_A;
				pszRevision = "2.0";
				break;

			case 0x0008:
				manufacturer = RaspberryPi::Manufacturer::SonyUK;
				memSize = RaspberryPi::MemorySize::_256Mb;
				model = RaspberryPi::Model::Rpi_Model_A;
				pszRevision = "2.0";
				break;

			case 0x0009:
				manufacturer = RaspberryPi::Manufacturer::Qisda;
				memSize = RaspberryPi::MemorySize::_256Mb;
				model = RaspberryPi::Model::Rpi_Model_A;
				pszRevision = "2.0";
				break;

			case 0x000D:
			case 0x000F:
				manufacturer = RaspberryPi::Manufacturer::Egoman;
				memSize = RaspberryPi::MemorySize::_512Mb;
				model = RaspberryPi::Model::Rpi_Model_B;
				pszRevision = "2.0";
				break;

			case 0x000E:
				manufacturer = RaspberryPi::Manufacturer::SonyUK;
				memSize = RaspberryPi::MemorySize::_512Mb;
				model = RaspberryPi::Model::Rpi_Model_B;
				pszRevision = "2.0";
				break;

			case 0x0010:
				manufacturer = RaspberryPi::Manufacturer::SonyUK;
				memSize = RaspberryPi::MemorySize::_512Mb;
				model = RaspberryPi::Model::Rpi_Model_B_plus;
				pszRevision = "1.2";
				break;

			case 0x0011:
				manufacturer = RaspberryPi::Manufacturer::SonyUK;
				memSize = RaspberryPi::MemorySize::_512Mb;
				model = RaspberryPi::Model::Compute_Module1;
				pszRevision = "1.0";
				break;

			case 0x0012:
				manufacturer = RaspberryPi::Manufacturer::SonyUK;
				memSize = RaspberryPi::MemorySize::_256Mb;
				model = RaspberryPi::Model::Rpi_Model_A_plus;
				pszRevision = "1.1";
				break;

			case 0x0013:
				manufacturer = RaspberryPi::Manufacturer::Embest;
				memSize = RaspberryPi::MemorySize::_512Mb;
				model = RaspberryPi::Model::Rpi_Model_B_plus;
				pszRevision = "1.2";
				break;

			case 0x0014:
				manufacturer = RaspberryPi::Manufacturer::Embest;
				memSize = RaspberryPi::MemorySize::_512Mb;
				model = RaspberryPi::Model::Compute_Module1;
				pszRevision = "1.0";
				break;

			case 0x0015:
				manufacturer = RaspberryPi::Manufacturer::Embest;
				memSize = RaspberryPi::MemorySize::_512Mb;
				model = RaspberryPi::Model::Rpi_Model_A_plus;
				pszRevision = "1.1";
				break;
		}
	}
	else {
		revisionIdCopy = revisionId;

		int rev = (int)(revisionIdCopy & 0x0F);
		sprintf(szRevision, "1.%d", rev);

		revisionIdCopy = revisionIdCopy >> 4;
		model = (RaspberryPi::Model)(revisionIdCopy & 0xFF);

		revisionIdCopy = revisionIdCopy >> 8;
		proc = (RaspberryPi::Processor)(revisionIdCopy & 0x0F);

		revisionIdCopy = revisionIdCopy >> 4;
		manufacturer = (RaspberryPi::Manufacturer)(revisionIdCopy & 0x0F);

		revisionIdCopy = revisionIdCopy >> 4;
		memSize = (RaspberryPi::MemorySize)(revisionIdCopy & 0x07);
	}

	uint32_t	baseAddress;

	switch (model) {
		case RaspberryPi::Model::Rpi_Model_A:
		case RaspberryPi::Model::Rpi_Model_B:
		case RaspberryPi::Model::Rpi_Model_A_plus:
		case RaspberryPi::Model::Rpi_Model_B_plus:
		case RaspberryPi::Model::Alpha_prototype:
		case RaspberryPi::Model::Compute_Module1:
		case RaspberryPi::Model::Rpi_Zero:
			baseAddress = RASPPI_PERI_BASE_OLD;
			break;

		case RaspberryPi::Model::Rpi_Model_2B:
		case RaspberryPi::Model::Rpi_Model_3B:
		case RaspberryPi::Model::Rpi_Model_3A_plus:
		case RaspberryPi::Model::Rpi_ZeroW:
		case RaspberryPi::Model::Compute_Module3:
		case RaspberryPi::Model::Compute_Module3_plus:
			baseAddress = RASPPI_PERI_BASE_NEW;
			break ;

		case RaspberryPi::Model::Rpi_Model_4B:
		case RaspberryPi::Model::Rpi_Model_400:
		case RaspberryPi::Model::Compute_Module4:
			baseAddress = RASPPI_PERI_BASE_4;
			break;

		default:
			throw new Exception(
					ERR_FILE_OPEN_FAILURE,
					"Unsupported Rpi Model",
					__FILE__,
					"PiFactory",
					"init()",
					__LINE__);
			break;
	}
	
	pi = new RaspberryPi(
					model,
					memSize,
					proc,
					manufacturer,
					szRevision,
					baseAddress);

	return pi;
}

uint32_t PiFactory::_getRevision()
{
	FILE *			cpuFd;
	char 			szLine[128];
	char *			c;
	uint32_t		revision;
	uint32_t		lineLength;

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

	while (fgets(szLine, 128, cpuFd) != NULL) {
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
		throw new Exception(
					ERR_INVALID_FILE_FORMAT,
					"No hex digit found at beginning of 'Revision' identifier",
					__FILE__,
					"PiFactory",
					"_getModel()",
					__LINE__);
	}

	revision = (uint32_t)strtol(c, NULL, 16); // Hex number with no leading 0x

	return revision;
}