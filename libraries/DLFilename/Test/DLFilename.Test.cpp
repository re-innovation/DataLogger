#include <iostream>

#include "DLFilename.h"

int main(int argc, char * argv[])
{

	Filename_setFromDate(4, 4, 14, 0);
	std::cout << Filename_get() << std::endl;

	Filename_setFromDate(4, 4, 14, 10);
	std::cout << Filename_get() << std::endl;

	Filename_setFromDate(4, 4, 14, 11);
	std::cout << Filename_get() << std::endl;

	Filename_setFromDate(4, 4, 14, 100);
	std::cout << Filename_get() << std::endl;

	Filename_setFromDate(4, 4, 14, 101);
	std::cout << Filename_get() << std::endl;

	Filename_setFromDate(4, 4, 14, 1000);
	std::cout << Filename_get() << std::endl;

	Filename_setFromDate(4, 4, 14, 1001);
	std::cout << Filename_get() << std::endl;

	return 0;

}