#pragma once
#include <array>

#include "../util.hpp" 
#include "Mos6502.hpp"

#define Push(x) Write(0x0100 + (m_uSP--), x)	// Pushes to stack
#define Pop() Read(0x0100 + (++m_uSP))			// Pops from stack

class Bus
{
public:
	Bus();
	~Bus();

	Mos6502 m_oCPU;
private:
	BYTE* m_pRAM;

public:
	void Write(WORD address, BYTE data);
	BYTE Read(WORD address, bool readOnly = false);

	std::string GetMemoryMap(WORD begin, WORD end);
};
