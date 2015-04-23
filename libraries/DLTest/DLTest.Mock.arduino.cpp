/* This is the entry point for mocked Arduino applications.
 * It accesses setup and loop via extern
 */
 
#include <time.h>

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

int main(void)
{
	setup();

	while (1)
	{
		loop();
	}
}