/*
 * C++ Library Includes
 */

#include <stdlib.h>

#include "DLTest.Mock.random.h"

long random(long min, long max)
{
    max--;
    return rand()%(max-min + 1) + min;
}

long random(long max)
{
    return random(0, max);
}
