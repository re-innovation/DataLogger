#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "DLUtility.AVR.h"
#include "DLUtility.HelperMacros.h"
#include "DLUtility.Readline.h"
#include "DLUtility.ArrayFunctions.h"

/*
 * DecToBcd
 * Converts a decimal value to its BCD equivalent. For example if the number 13
 * is passed in, the function will return (1 * 16) + (3 * 1) = 19.
 */
char DecToBcd(char value);

#endif