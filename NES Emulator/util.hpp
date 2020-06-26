#pragma once
#include <cstdint>
#include <iomanip>

// Just some typedefs and other utility
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

// Some utility macros
#define IS_IN_RANGE( x, low, high ) (((low <= x) && (x <= high)) ? true : false)

#define MAKE(x, y, z) {#x, x, y, z}		// Creates an Instruction initializer list
#define SERVE(x) return m_uFetched = x	// Returns the given value and sets fetch
#define TO_WORD(lo, hi) ((static_cast<WORD>(hi) << 8) | lo)	// Assembles a WORD from two BYTES
#define HEX(p, x, w) p << std::setfill('0') << std::setw(w) << std::hex << std::uppercase << (WORD)x // fancy hex output

#define BIT_(n, x) (x & (1 << n))	// Masks the input with the bit at position n