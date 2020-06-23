#pragma once
#include <cstdint>

// Just some typedefs and other utility
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

// Some utility macros
#define IS_IN_RANGE( x, low, high ) (((low <= x) && (x <= high)) ? true : false)