
#ifndef _INCL_EXCEPTION
#define _INCL_EXCEPTION

#define MESSAGE_BUFFER_SIZE			1024
#define EXCEPTION_BUFFER_SIZE		2048

#define	ERR_MALLOC					0x00000001
#define ERR_INDEX_OUT_OF_RANGE		0x00000002
#define ERR_MEMORY_MAP_ERROR		0x00000004
#define ERR_ARRAY_OVERFLOW			0x00000008
#define ERR_DEVICE_OPEN_FAILURE		0x00000010
#define ERR_PERIPHERAL_IN_USE		0x00000020
#define ERR_INSUFFICIENT_PRIVILEGES	0x00000040
#define ERR_NO_TIMERS_AVAILABLE		0x00000080
#define ERR_FILE_OPEN_FAILURE		0x00000100
#define ERR_INVALID_FILE_FORMAT		0x00000200
#define ERR_SCHEDULE				0x00000400

#define ERR_UNDEFINED				0x0000FFFF


class Exception
{
	private:
		int				errorCode;
		char			szMessage[MESSAGE_BUFFER_SIZE];
		
		char			szFileName[256];
		char			szClassName[128];
		char			szMethodName[128];
		
		char 			exception[EXCEPTION_BUFFER_SIZE];
		
		unsigned int	lineNumber;
		
		void			_initialise();
		
	public:
						Exception();
						Exception(const char *pszMessage);
						Exception(int errorCode, const char *pszMessage);
						Exception(
								int errorCode, 
								const char *pszMessage, 
								const char *pszFileName, 
								const char *pszClassName, 
								const char *pszMethodName, 
								unsigned int lineNumber);
							
		int				getErrorCode();
		unsigned int	getLineNumber();
		
		char *		getFileName();
		char *		getClassName();
		char *		getMethodName();
		
		char *		getMessage();
		
		char *		getExceptionString();
};

#endif
