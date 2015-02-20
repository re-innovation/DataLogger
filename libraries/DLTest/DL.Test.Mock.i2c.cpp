/*
 * C++ Library Includes
 */

#include <stdint.h>

#include "DL.Test.Mock.i2c.h"

void I2CMock::begin(void) {}
void I2CMock::beginTransmission(uint8_t addr) { m_addr = addr; }
void I2CMock::endTransmission(void) {}

void I2CMock::requestFrom(uint8_t addr, uint8_t qty) {(void)addr; (void)qty; }

uint8_t I2CMock::receive(void)
{
	m_recvd++;
	return m_recvdBuffer ?  *m_recvdBuffer++ : 0;
}

void I2CMock::send(uint8_t toSend)
{
	m_sent++;
	if (m_sentBuffer)
	{
		*m_sentBuffer++ = toSend;
	}

}

I2CMock Wire;