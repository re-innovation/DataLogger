#ifndef _DLUTILITY_HELPER_MACROS_H_
#define _DLUTILITY_HELPER_MACROS_H_

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

// Simple min and max macros
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
 
// Returns TRUE if - assuming i is loop counter var - this is the last time a loop will run.
#define lastinloop(i, loopmax) ((i == (loopmax - 1)))

// Increment towards a maximum and rollover to zero
#define incrementwithrollover(var, max) (var = (var < max) ? var + 1 : 0)
// Decrement towards zero and rollover to a maximum
#define decrementwithrollover(var, max) (var = (var > 0) ? var - 1 : max)

// Increment towards a maximum and rollover to a minimum
// e.g repeating incrementwithminmax(x, 5, 10) gives x values 5, 6, 7, 8, 9, 10, 5, 6...
#define incrementwithminmax(var, min, max) (var = (var < max) ? var + 1 : min)

// Decrement towards a minimum and rollover to a minimum
// e.g repeating decrementwithminmax(x, 5, 10) gives x values 10, 9, 8, 7, 6, 5, 10, 9...
#define decrementwithminmax(var, max) (var = (var > 0) ? var - 1 : max)

// Set or clear individual register bits
#define bset(reg, bit) (reg |= (1 << bit))
#define bclr(reg, bit) (reg &= ~(1 << bit))

// Division with rounding up or down (for both +ve and -ve numbers)
#define div_round_pos(x, y) (((x) + ((y)/2))/(y))
#define div_round_neg(x, y) (((x) - ((y)/2))/(y))

// Division with rounding up or down (when +ve or -ve not known)
#define div_round(x, y) (x == 0) ? 0 : ((x > 0) ? div_round_pos(x, y) : div_round_neg(x, y))

// Number of elements in an array
#define N_ELE(x) (sizeof(x)/sizeof(x[0]))

#endif