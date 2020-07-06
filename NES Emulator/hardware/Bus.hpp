#pragma once
#include <array>
#include <memory>

#include "../util.hpp" 
#include "Mos6502.hpp"
#include "RP2C02.hpp"
#include "Cartridge.hpp"

class NESWindow;

class Bus
{
public:
	Bus(NESWindow* parentWindow);
	~Bus();

	Mos6502 m_oCPU;
	RP2C02 m_oPPU;
	std::shared_ptr<Cartridge> m_pCartridge;
private:
	BYTE* m_pCPURAM;

public:
	void WriteCPU(WORD address, BYTE data);
	BYTE ReadCPU(WORD address, bool readOnly = false);

public:
	void InsertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void Reset();
	void Clock();

private:

	uint64_t m_uClockCounter;

	NESWindow* m_pParentWindow;
};
