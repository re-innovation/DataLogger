#ifndef _TEST_MOCK_ARDUINO_H_
#define _TEST_MOCK_ARDUINO_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "DLTest.Mock.delay.h"
#include "DLTest.Mock.Serial.h"

#define HIGH (1)
#define LOW (0)

#define OUTPUT (0)
#define INPUT (1)
#define INPUT_PULLUP (2)

#define A0 (0)
#define A1 (1)
#define A2 (2)

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value); 
unsigned long millis(void);

void _exitMock(void);

#endif
