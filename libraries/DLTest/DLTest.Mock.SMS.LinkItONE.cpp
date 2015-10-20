/*
 * C++ Library Includes
 */

#include <stdint.h>

#include "DLSMS.h"
#include "DLSMS.linkitone.h"

LinkItOneSMSInterface::LinkItOneSMSInterface() {}
LinkItOneSMSInterface::~LinkItOneSMSInterface() {}

bool LinkItOneSMSInterface::send(char * pNumber, char * pMessage)
{
	(void)pNumber;
	(void)pMessage;
	return true;
}