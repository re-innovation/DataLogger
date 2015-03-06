#ifndef _AVR_UTILITY_H_
#define _AVR_UTILITY_H_

	#ifdef AVR_ARCH
		/*
		 * getProgmemString
		 * An AVR-specific function that copies a string from program memory into RAM
		 * So it can be used.
		 */

		char* getProgmemString(const char* str);

	#endif // AVR_ARCH
#endif //_AVR_UTILITY_H_
