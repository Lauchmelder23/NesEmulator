#pragma once
#include <cstdint>
#include <iomanip>

// Just some typedefs and other utility
typedef uint8_t BYTE;
typedef uint16_t WORD;

// Some utility macros
#define IS_IN_RANGE( x, low, high ) (((low <= x) && (x <= high)))

#define MAKE(x, y, z) {#x, x, y, z}		// Creates an Instruction initializer list
#define MAKE_ILLEGAL(w, x, y, z) {w, x, y, z}

#define SERVE(x) return m_uFetched = x	// Returns the given value and sets fetch
#define TO_WORD(lo, hi) ((static_cast<WORD>(hi) << 8) | lo)	// Assembles a WORD from two BYTES
#define HEX(p, x, w) p << std::setfill('0') << std::setw(w) << std::hex << std::uppercase << (WORD)x // fancy hex output

#define BIT_(n, x) ((x & (1 << n)) >> n)	// Masks the input with the bit at position n
#define HI(x) ((x & 0xFF00) >> 8)	// Gets the high byte of a WORD
#define LO(x) (x & 0x00FF)			// Gets the low byte of a WORD


#define PushTo(x) Write(0x0100 + (m_uSP--), x)	// Pushes to stack
#define PopFrom() Read(0x0100 + (++m_uSP))			// Pops from stack

#ifdef WIN32
	#define STRERROR(buf, len, err) strerror_s(buf, len, err)
#else
	#define STRERROR(buf, len, err) strerror_r(err, buf, len)
#endif	// WIN32