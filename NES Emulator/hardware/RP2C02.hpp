#pragma once

#include "../util.hpp"

#include "Cartridge.hpp"

class RP2C02
{
public:
	RP2C02();
	~RP2C02();

	BYTE ReadCPU(WORD address, bool readonly);
	void WriteCPU(WORD address, BYTE value);

	BYTE ReadPPU(WORD address, bool readonly);
	void WritePPU(WORD address, BYTE value);

private:
	const Cartridge* m_pCartridge;	

	// Connected stuff

	// 2x 1KB nametables
	BYTE* m_pNameTables;

	// 32 Bytes of Palette info
	BYTE* m_pPaletteTable;

public:
	void InsertCartridge(const Cartridge* cartridge);
	void Clock();
};