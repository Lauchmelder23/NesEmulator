#pragma once
#include <array>

#include "../util.hpp" 
#include "Mos6502.hpp"

class Bus
{
public:
	Bus();
	~Bus();

private:
	Mos6502 m_oCPU;
	BYTE* m_pRAM;

public:
	void Write(WORD address, BYTE data);
	BYTE Read(WORD address, bool readOnly = false);
};
