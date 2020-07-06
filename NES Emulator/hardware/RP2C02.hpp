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

	// Internal register
	union 
	{
		struct
		{
			WORD CoarseX : 5;
			WORD CoarseY : 5;
			WORD Nametable : 2;
			WORD FineY : 3;
			WORD Unused : 1;
		};

		WORD Raw = 0x0000;
	} m_oVramRegister, m_oTempVramRegister;
	BYTE m_nFineX = 0x00;

	struct
	{
		BYTE ID = 0x00;
		BYTE Attrib = 0x00;
		BYTE LSB = 0x00;
		BYTE MSB = 0x00;
	} m_oBgNextTileInfo;

	struct  
	{
		WORD PatternLo = 0x00;
		WORD PatternHi = 0x00;
		WORD AttribLo = 0x00;
		WORD AttribHi = 0x00;
	} m_oBgShiftRegister;

public:
	RP2C02();
	~RP2C02();

	void Initialize(SDL_Renderer* renderer);

	BYTE ReadCPU(WORD address, bool readonly = true);
	void WriteCPU(WORD address, BYTE value);

	BYTE ReadPPU(WORD address, bool readonly = true);
	void WritePPU(WORD address, BYTE value);

	void InsertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void Tick();

	SDL_Color& PatternPixelScreenColour(BYTE paletteID, BYTE pixelValue);

public:
	std::shared_ptr<Cartridge> m_pCartridge;	

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
};