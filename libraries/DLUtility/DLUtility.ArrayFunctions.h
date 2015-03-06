#ifndef _UTILITY_ARRAY_FUNCTIONS_H_
#define _UTILITY_ARRAY_FUNCTIONS_H_

/*
 * Functions for working with numeric arrays
 *
 * - getMaximum, getMinimum: Returns the largest/smallest value from array
 * * getIndexOfMaximum: Returns the index of the highest value in array
 * - fillArray: like memset, but type aware so no working out of size in bytes is required
 */

template <typename T>
T getMaximum(T * array, uint16_t length);

template <typename T>
uint16_t getIndexOfMaximum(T * array, uint16_t length);

template <typename T>
void fillArray(T * array, T value, uint16_t size);

#endif
