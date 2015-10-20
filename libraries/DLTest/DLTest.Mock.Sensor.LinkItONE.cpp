/*
 * C++ Library Includes
 */

#include <stdint.h>

#include "DLSensor.LinkItONE.h"
#include "DLTest.Mock.random.h"

LinkItONEADC::LinkItONEADC(uint8_t channel)
{
	(void)channel;
}

uint16_t LinkItONEADC::read(void)
{
	return 0;
}
