/*
 * C++ Library Includes
 */

#include <stdint.h>

#include "DLSMS.h"
#include "DLSMS.LinkItONE.h"

LinkItOneSMSInterface::LinkItOneSMSInterface() {}
LinkItOneSMSInterface::~LinkItOneSMSInterface() {}

bool LinkItOneSMSInterface::send(char * pNumber, char * pMessage)
{
	(void)pNumber;
	(void)pMessage;
	return true;
}