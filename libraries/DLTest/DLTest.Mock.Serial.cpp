/*
 * C++ Library Includes
 */

#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#include "DLTest.Mock.Serial.h"

void _Serial::begin(int baudrate) { (void)baudrate; }

template <typename T>
void _Serial::print(T toPrint)
{
    std::cout << toPrint;
}

template <typename T>
void _Serial::println(T toPrint)
{
    print(toPrint);
    std::cout << std::endl;
}

template <>
void _Serial::print<uint8_t>(uint8_t toPrint)
{
    std::cout << (int)toPrint;
}

template <>
void _Serial::println<uint8_t>(uint8_t toPrint)
{
    std::cout << (int)toPrint << std::endl;
}

template <>
void _Serial::print<int8_t>(int8_t toPrint)
{
    std::cout << (int)toPrint;
}

template <>
void _Serial::println<int8_t>(int8_t toPrint)
{
    std::cout << (int)toPrint << std::endl;
}

void _Serial::println(void)
{
    std::cout << std::endl;
}

template void _Serial::print<char *>(char * toPrint);
template void _Serial::println<char *>(char * toPrint);

template void _Serial::print<char>(char toPrint);
template void _Serial::println<char>(char toPrint);

template void _Serial::print<bool>(bool toPrint);
template void _Serial::println<bool>(bool toPrint);

template void _Serial::print<char const *>(char const * toPrint);
template void _Serial::println<char const *>(char const * toPrint);

template void _Serial::print<uint16_t>(uint16_t toPrint);
template void _Serial::println<uint16_t>(uint16_t toPrint);

template void _Serial::print<int16_t>(int16_t toPrint);
template void _Serial::println<int16_t>(int16_t toPrint);

template void _Serial::print<uint32_t>(uint32_t toPrint);
template void _Serial::println<uint32_t>(uint32_t toPrint);

template void _Serial::print<int32_t>(int32_t toPrint);
template void _Serial::println<int32_t>(int32_t toPrint);

template void _Serial::print<long>(long toPrint);
template void _Serial::println<long>(long toPrint);

template void _Serial::print<unsigned long>(unsigned long toPrint);
template void _Serial::println<unsigned long>(unsigned long toPrint);

template void _Serial::print<float>(float toPrint);
template void _Serial::println<float>(float toPrint);

_Serial Serial;
