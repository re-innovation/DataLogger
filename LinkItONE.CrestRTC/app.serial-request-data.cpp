/*
 * C/C++ Includes
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <LDateTime.h>

/*
 * Library Includes
 */

#include "DLUtility.h"

/*
 * Application Includes
 */

#include "app.serial-request-data.h"

/*
 * Public Functions
 */

bool APP_SerialRequestData_FormatArray(float * data, int n_fields, char * buffer, int buffer_length)
{
	if (!data) { return false; }
	if (!buffer) { return false; }
	if (n_fields == 0) { return false; }

	FixedLengthAccumulator accumulator(buffer, buffer_length);
		
	accumulator.writeChar('s');

	int i;
	char sprintf_buffer[10];

  unsigned int rtca;
  LDateTime.getRtc(&rtca);
  sprintf(sprintf_buffer, "%x", rtca);
  accumulator.writeString(sprintf_buffer);
  accumulator.writeChar(',');
  
	for (i = 0; i < n_fields; i++)
	{
		sprintf(sprintf_buffer, "%.1f", data[i]);
		accumulator.writeString(sprintf_buffer);
		accumulator.writeChar(',');
	}
	accumulator.remove(1);
	accumulator.writeChar('t');

	return true;
}
