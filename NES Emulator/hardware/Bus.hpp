#pragma once
#include <array>

#include "../util.hpp" 
#include "Mos6502.hpp"
#include "RP2C02.hpp"
#include "Cartridge.hpp"

class Bus
{
public:
	Bus();
	~Bus();

	Mos6502 m_oCPU;
	RP2C02 m_oPPU;
	Cartridge* m_pCartridge;
private:
	BYTE* m_pCPURAM;

public:
	void WriteCPU(WORD address, BYTE data);
	BYTE ReadCPU(WORD address, bool readOnly = false);

public:
	void InsertCartridge(Cartridge* cartridge);
	void Reset();
	void Clock();

	std::string GetMemoryMap(WORD begin, WORD end);

private:
	uint64_t m_uClockCounter;
};
