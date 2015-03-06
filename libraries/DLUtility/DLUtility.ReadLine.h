#ifndef _UTILITY_READLINE_H_
#define _UTILITY_READLINE_H_

/*
 * READFN type
 * - Used to pass a function pointer to standard Read* functions
 */
typedef char (*READFN)(void);

/*
 * ReadLineWithReadFunction
 * Reads a single line from a stream-like source.
 * As this utility library may work with may different non-standard file access
 * libraries, the function is designed to work with a pure-C READFN function pointer.
 * The function provided should return each character from a file/stream/whatever
 * in sequence unless EOF is reached (or the file is otherwise unavailable), when it
 * should return \0.
 *
 * The function will read from fn into buffer up to a maximum of n chars. If there is
 * room, it will append a null pointer. If stripCRLF is true, '\r'  and '\n' chars will
 * be stripped from the output.
 *
 * The function returns the number of chars written to buffer excluding any NULL pointer.
 */
uint32_t ReadLineWithReadFunction(READFN fn, char * buffer, uint32_t n, bool stripCRLF = false);

#endif
