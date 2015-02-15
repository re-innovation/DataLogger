#ifndef _UTILITY_H_
#define _UTILITY_H_

/*
 * READFN type
 * - Used to pass a function pointer to standard Read* functions
 */
typedef char (*READFN)(void);

uint32_t ReadLineWithReadFunction(READFN fn, char * buffer, uint32_t n, bool stripCRLF = false);

#endif