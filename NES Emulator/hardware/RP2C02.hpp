#pragma once

#include <SDL.h>

#include "../util.hpp"

#include "Cartridge.hpp"

class RP2C02
{
private:
	// The sacred registers!
	// CONTROL
	union 
	{
		struct 
		{
			BYTE BaseNametableAddress : 2;
			BYTE VRAMAddressIncrement : 1;
			BYTE SpritePatternTable : 1;
			BYTE BackgroundPatternTable : 1;
			BYTE SpriteSize : 1;
			BYTE Master : 1;
			BYTE GenerateNMI : 1;
		};

		BYTE Raw;
	} m_oControl;

	// MASK
	union 
	{
		struct
		{
			BYTE Greyscale : 1;
			BYTE ShowBackgroundLeft : 1;
			BYTE ShowSpritesLeft : 1;
			BYTE ShowBackground : 1;
			BYTE ShowSprites : 1;
			BYTE BoostRed : 1;
			BYTE BoostGreen : 1;
			BYTE BoostBlue : 1;
		};

		BYTE Raw;
	} m_oMask;

	// STATUS
	union 
	{
		struct 
		{
			BYTE Unused : 5;
			BYTE SpriteOverflow : 1;
			BYTE SpriteZeroHit : 1;
			BYTE VBlank : 1;
		};

		BYTE Raw;
	} m_oStatus;

public:
	RP2C02();
	~RP2C02();

	void Initialize(SDL_Renderer* renderer);

	BYTE ReadCPU(WORD address, bool readonly = true);
	void WriteCPU(WORD address, BYTE value);

	BYTE ReadPPU(WORD address, bool readonly = true);
	void WritePPU(WORD address, BYTE value);

	void InsertCartridge(Cartridge* cartridge);
	void Tick();

	SDL_Color& PatternPixelScreenColour(BYTE paletteID, BYTE pixelValue);

private:
	Cartridge* m_pCartridge;	

	// Connected stuff

	// 2x 1KB nametables
	BYTE** m_pNameTables;

	// 32 Bytes of Palette info
	BYTE* m_pPaletteTable;

private:
	SDL_Renderer* m_pRenderer;

	SDL_Color m_pPalette[0x40];
	SDL_Texture* m_pScreen;
	SDL_Texture** m_pTexNameTables;
	SDL_Texture** m_pTexPatternTables;

public:
	SDL_Texture* GetScreen();
	SDL_Texture* GetNameTable(BYTE i);
	SDL_Texture* GetPatternTable(BYTE i, BYTE palette);
	int16_t GetScanline() { return m_nScanline; }
	int16_t GetCycle() { return m_nCycle; }

	bool isFrameComplete = false;
	bool isThrowingInterrupt = false;

private:
	int16_t m_nScanline, m_nCycle;

	BYTE m_nAddressLatch = 0x00;
	BYTE m_nPPUBuffer = 0x00;	// Data transfer is delayed by 1 cycle
	WORD m_nPPUAddress = 0x0000;
};