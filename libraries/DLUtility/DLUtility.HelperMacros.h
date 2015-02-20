#ifndef _UTILITY_HELPER_MACROS_H_
#define _UTILITY_HELPER_MACROS_H_

/*
 * BETWEENx macros
 * Abstracts away the slightly messy/hard-to-read/easy-to-get-wrong
 * logic of working out whether x is between a and b.
 *
 * Exclusive and inclusive macros are defined.
 */

#define BETWEEN_INC(x, a, b) ((x >= a) && (x <= b))
#define BETWEEN_EXC(x, a, b) ((x > a) && (x < b))

#define BETWEEN_EXC_HI(x, a, b) ((x >= a) && (x < b))
#define BETWEEN_EXC_LO(x, a, b) ((x > a) && (x <= b))

 #endif