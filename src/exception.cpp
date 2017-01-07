#include <string.h>
#include <stdio.h>

#include "exception.h"

void Exception::_initialise()
{
	this->szMessage[0]		= '\0';
	this->szFileName[0] 	= '\0';
	this->szClassName[0] 	= '\0';
	this->szMethodName[0] 	= '\0';
	
	this->errorCode = 0;
	this->lineNumber = 0;
}

Exception::Exception()
{
	_initialise();
	this->errorCode = ERR_UNDEFINED;
	strcpy(this->szMessage, "Undefined exeption");
}

Exception::Exception(const char *pszMessage)
{
	_initialise();
	strcpy(this->szMessage, pszMessage);
}

Exception::Exception(int errorCode, const char *pszMessage)
{
	_initialise();
	this->errorCode = errorCode;
	strcpy(this->szMessage, pszMessage);
}

Exception::Exception(int errorCode, const char *pszMessage, const char *pszFileName, const char *pszClassName, const char *pszMethodName, unsigned int lineNumber)
{
	_initialise();
	this->errorCode = errorCode;
	strcpy(this->szMessage, pszMessage);
	strcpy(this->szFileName, pszFileName);
	strcpy(this->szClassName, pszClassName);
	strcpy(this->szMethodName, pszMethodName);
	this->lineNumber = lineNumber;
}

int Exception::getErrorCode()
{
	return this->errorCode;
}

unsigned int Exception::getLineNumber()
{
	return this->lineNumber;
}

char *Exception::getFileName()
{
	return this->szFileName;
}

char *Exception::getClassName()
{
	return this->szClassName;
}

char *Exception::getMethodName()
{
	return this->szMethodName;
}

char *Exception::getMessage()
{
	return this->szMessage;
}

char *Exception::getExceptionString()
{
	char		szLineNum[8];
	
	exception[0] = '\0';
	
	if (errorCode) {
		sprintf(exception, "*** Exception (0x%04X) : ", (unsigned int)errorCode);
	}
	else {
		strcat(exception, "*** Exception : ");
	}
	
	strcat(exception, szMessage);
	strcat(exception, " ***");
	
	if (strlen(szFileName) > 0 && strlen(szClassName) > 0 && strlen(szMethodName) > 0) {
		strcat(exception, "\nIn ");
		strcat(exception, szFileName);
		strcat(exception, " - ");
		strcat(exception, szClassName);
		strcat(exception, "::");
		strcat(exception, szMethodName);
	}
	
	if (lineNumber > 0) {
		strcat(exception, " at line ");
		sprintf(szLineNum, "%d", lineNumber);
		strcat(exception, szLineNum);
	}

	return exception;
}
