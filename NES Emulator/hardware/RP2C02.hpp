#pragma once

#include <SDL.h>

#include "../util.hpp"

#include "Cartridge.hpp"

class RP2C02
{
public:
	RP2C02();
	~RP2C02();

	void Initialize(SDL_Renderer* renderer);

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
	void Tick();

private:
	SDL_Renderer* m_pRenderer;

	SDL_Color m_pPalette[0x40];
	SDL_Texture* m_pScreen;
	SDL_Texture** m_pTexNameTables;
	SDL_Texture** m_pTexPatternTables;

public:
	SDL_Texture* GetScreen();
	SDL_Texture* GetNameTable(BYTE i);
	SDL_Texture* GetPatternTable(BYTE i);

	bool isFrameComplete = false;

private:
	int16_t m_nScanline, m_nCycle;
};