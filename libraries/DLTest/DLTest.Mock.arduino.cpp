/* This is the entry point for mocked Arduino applications.
 * It accesses setup and loop via extern
 */

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <iostream>
 
#include "DLTest.Mock.arduino.h"

extern void setup(void);
extern void loop(void);

unsigned long millis(void)
{
	clock_t clock_ticks = clock();
	return (clock_ticks * 1000) / CLOCKS_PER_SEC;
}

void pinMode(uint8_t pin, uint8_t mode)
{
	(void)pin; (void)mode;	
}

void digitalWrite(uint8_t pin, uint8_t value)
{
	(void)pin; (void)value;
}

void _exitMock(void)
{
	std::cout.flush();
	exit(1);
}

int main(void)
{
	setup();


	int32_t runLoopCount;
	do
	{
		std::cout << "Enter number of times to run loop: ";
		std::cin >> runLoopCount;
		std::cout << "Running loop " << runLoopCount << " times..." << std::endl;
		while (--runLoopCount > 0)
		{
			loop();
		}
	} while (runLoopCount >= 0);
}