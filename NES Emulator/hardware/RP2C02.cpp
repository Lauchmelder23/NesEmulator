#include "RP2C02.hpp"

#include <memory>

RP2C02::RP2C02() :
	m_pCartridge(nullptr), m_pNameTables(nullptr), m_pPaletteTable(nullptr), m_nScanline(0), m_nCycle(0)
{
	m_pPalette[0x00] = {84, 84, 84};
	m_pPalette[0x01] = {0, 30, 116};
	m_pPalette[0x02] = {8, 16, 144};
	m_pPalette[0x03] = {48, 0, 136};
	m_pPalette[0x04] = {68, 0, 100};
	m_pPalette[0x05] = {92, 0, 48};
	m_pPalette[0x06] = {84, 4, 0};
	m_pPalette[0x07] = {60, 24, 0};
	m_pPalette[0x08] = {32, 42, 0};
	m_pPalette[0x09] = {8, 58, 0};
	m_pPalette[0x0A] = {0, 64, 0};
	m_pPalette[0x0B] = {0, 60, 0};
	m_pPalette[0x0C] = {0, 50, 60};
	m_pPalette[0x0D] = {0, 0, 0},
	m_pPalette[0x0E] = {0, 0, 0},
	m_pPalette[0x0F] = {0, 0, 0},

	m_pPalette[0x10] = { 152, 150, 152 };
	m_pPalette[0x11] = { 8, 76, 196 };
	m_pPalette[0x12] = { 48, 50, 236 };
	m_pPalette[0x13] = { 92, 30, 228 };
	m_pPalette[0x14] = { 136, 20, 176 };
	m_pPalette[0x15] = { 160, 20, 100 };
	m_pPalette[0x16] = { 152, 34, 32 };
	m_pPalette[0x17] = { 120, 60, 0 };
	m_pPalette[0x18] = { 84, 90, 0 };
	m_pPalette[0x19] = { 40, 114, 0 };
	m_pPalette[0x1A] = { 8, 124, 0 };
	m_pPalette[0x1B] = { 0, 118, 40 };
	m_pPalette[0x1C] = { 0, 102, 120 };
	m_pPalette[0x1D] = { 0, 0, 0 },
	m_pPalette[0x1E] = { 0, 0, 0 },
	m_pPalette[0x1F] = { 0, 0, 0 },


	m_pPalette[0x20] = { 236, 238, 236 };
	m_pPalette[0x21] = { 76, 154, 236 };
	m_pPalette[0x22] = { 120, 124, 236 };
	m_pPalette[0x23] = { 176, 98, 236 };
	m_pPalette[0x24] = { 228, 84, 236 };
	m_pPalette[0x25] = { 236, 88, 180 };
	m_pPalette[0x26] = { 236, 106, 100 };
	m_pPalette[0x27] = { 212, 136, 32 };
	m_pPalette[0x28] = { 160, 170, 0 };
	m_pPalette[0x29] = { 116, 196, 0 };
	m_pPalette[0x2A] = { 76, 208, 32 };
	m_pPalette[0x2B] = { 56, 204, 108 };
	m_pPalette[0x2C] = { 56, 180, 204 };
	m_pPalette[0x2D] = { 60, 60, 60 },
	m_pPalette[0x2E] = { 0, 0, 0 },
	m_pPalette[0x2F] = { 0, 0,60 },

	m_pPalette[0x30] = { 236, 238, 236 };
	m_pPalette[0x31] = { 168, 204, 236 };
	m_pPalette[0x32] = { 188, 188, 236 };
	m_pPalette[0x33] = { 212, 178, 236 };
	m_pPalette[0x34] = { 236, 174, 236 };
	m_pPalette[0x35] = { 236, 174, 212 };
	m_pPalette[0x36] = { 236, 180, 176 };
	m_pPalette[0x37] = { 228, 196, 144 };
	m_pPalette[0x38] = { 204, 210, 120 };
	m_pPalette[0x39] = { 180, 222, 120 };
	m_pPalette[0x3A] = { 168, 226, 144 };
	m_pPalette[0x3B] = { 152, 226, 180 };
	m_pPalette[0x3C] = { 160, 214, 228 };
	m_pPalette[0x3D] = { 160, 162, 160 };
	m_pPalette[0x3E] = { 0, 0, 0 };
	m_pPalette[0x3F] = { 0, 0, 0 };

	m_pNameTables = new BYTE[2 * 1024];
	m_pPaletteTable = new BYTE[32];
}

RP2C02::~RP2C02()
{
	delete[] m_pPaletteTable;
	m_pPaletteTable = nullptr;

	delete[] m_pNameTables;
	m_pNameTables = nullptr;

	SDL_DestroyTexture(m_pTexPatternTables[0]);
	SDL_DestroyTexture(m_pTexPatternTables[1]);
	delete[] m_pTexPatternTables;
	m_pTexPatternTables = nullptr;

	SDL_DestroyTexture(m_pTexNameTables[0]);
	SDL_DestroyTexture(m_pTexNameTables[1]);
	delete[] m_pTexNameTables;
	m_pTexNameTables = nullptr;

	SDL_DestroyTexture(m_pScreen);
}

void RP2C02::Initialize(SDL_Renderer* renderer)
{
	m_pRenderer = renderer;

	m_pScreen = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 256, 240);

	m_pTexNameTables = new SDL_Texture * [2]{
		SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 256, 240),
		SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 256, 240)
	};

	m_pTexPatternTables = new SDL_Texture * [2]{
		SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 128, 128),
		SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 128, 128)
	};
}

BYTE RP2C02::ReadCPU(WORD address, bool readonly)
{
	BYTE data = 0x00;

	// Just dummy stuff for now
	// TODO: Implement PPU registers
	switch (address)
	{
	case 0x0000:	// PPUCTRL
		break;

	case 0x0001:	// PPUMASK
		break;

	case 0x0002:	// PPUSTATUS
		// TODO: remove
		m_oStatus.VBlank = 1;
		data = (m_oStatus.Raw & 0xE0) | (m_nPPUBuffer & 0x1F);
		m_oStatus.VBlank = 0;
		m_nAddressLatch = 0x00;
		break;

	case 0x0003:	// OAMADDR
		break;
	case 0x0004:	// OAMDATA
		break;
	case 0x0005:	// PPUSCROLL
		break;

	case 0x0006:	// PPUADDR
		// Does nothing
		break;

	case 0x0007:	// PPUDATA
		data = m_nPPUBuffer;	// Data transfer needs to be buffered. It's delayed by 1 cycle
		m_nPPUBuffer = ReadPPU(m_nPPUAddress);

		// EXCEPT the palette data :)
		if (m_nPPUAddress > 0x3F00) 
			data = m_nPPUBuffer;
		break;
	}

	return data;
}

void RP2C02::WriteCPU(WORD address, BYTE value)
{
	// Just dummy stuff for now
	// TODO: Implement PPU registers
	switch (address)
	{
	case 0x0000:	// PPUCTRL
		m_oControl.Raw = value;
		break;

	case 0x0001:	// PPUMASK
		m_oMask.Raw = value;
		break;

	case 0x0002:	// PPUSTATUS
		// Status cant be written to
		break;

	case 0x0003:	// OAMADDR
		break;
	case 0x0004:	// OAMDATA
		break;
	case 0x0005:	// PPUSCROLL
		break;
	case 0x0006:	// PPUADDR
		if (m_nAddressLatch == 0x00)
		{
			m_nPPUAddress = (m_nPPUAddress & 0x00FF) | (value << 8);
			m_nAddressLatch = 0x01;
		}
		else
		{
			m_nPPUAddress = (m_nPPUAddress & 0xFF00) | value;
			m_nAddressLatch = 0x00;
		}
		break;

	case 0x0007:	// PPUDATA
		WritePPU(m_nPPUAddress, value);
		break;
	}
}

BYTE RP2C02::ReadPPU(WORD address, bool readonly)
{
	BYTE data = 0x00;
	address &= 0x3FFF;

	if (m_pCartridge->ReadPPU(address, data))
	{

	}

	// Nametables
	else if (IS_IN_RANGE(address, 0x2000, 0x3EFF))
	{

	}

	// Palette information
	else if (IS_IN_RANGE(address, 0x3F00, 0x3FFF))
	{
		address &= 0x001F;

		// Hardcode in the mirroring because I'm lazy
		// TODO: figure this out
		if (address == 0x0010) address = 0x0000;
		if (address == 0x0014) address = 0x0004;
		if (address == 0x0018) address = 0x0008;
		if (address == 0x001C) address = 0x000C;

		data = m_pPaletteTable[address];
	}

	return data;
}

void RP2C02::WritePPU(WORD address, BYTE value)
{
	address &= 0x3FFF;

	if (m_pCartridge->WritePPU(address, value))
	{

	}

	// Nametables
	else if (IS_IN_RANGE(address, 0x2000, 0x3EFF))
	{

	}

	// Palette information
	else if (IS_IN_RANGE(address, 0x3F00, 0x3FFF))
	{
		address &= 0x001F;

		// Hardcode in the mirroring because I'm lazy
		// TODO: figure this out
		if (address == 0x0010) address = 0x0000;
		if (address == 0x0014) address = 0x0004;
		if (address == 0x0018) address = 0x0008;
		if (address == 0x001C) address = 0x000C;

		m_pPaletteTable[address] = value;
	}
}

void RP2C02::InsertCartridge(Cartridge* cartridge)
{
	m_pCartridge = cartridge;
}

void RP2C02::Tick()
{
	SDL_Color c = m_pPalette[(rand() % 2) ? 0x3F : 0x30];
	SDL_SetRenderDrawColor(m_pRenderer, c.r, c.g, c.b, 255);
	SDL_RenderDrawPoint(m_pRenderer, m_nCycle - 1, m_nScanline);

	m_nCycle++;	
	// Ugly hardcoding
	if (m_nCycle >= 341)
	{
		m_nCycle = 0;
		m_nScanline++;

		if (m_nScanline >= 261)
		{
			m_nScanline = -1;
			isFrameComplete = true;
		}
	}
	
}

SDL_Color& RP2C02::PatternPixelScreenColour(BYTE paletteID, BYTE pixelValue)
{
	return m_pPalette[ReadPPU(0x3F00 + (paletteID << 2) + pixelValue)];
}

SDL_Texture* RP2C02::GetScreen()
{
	return m_pScreen;
}

SDL_Texture* RP2C02::GetNameTable(BYTE i)
{
	return m_pTexNameTables[i];
}

SDL_Texture* RP2C02::GetPatternTable(BYTE i, BYTE palette)
{
	SDL_Texture* prevRenderTarget = SDL_GetRenderTarget(m_pRenderer);
	SDL_SetRenderTarget(m_pRenderer, m_pTexPatternTables[i]);

	// A pattern table is 16x16 "tiles"
	for (BYTE tileY = 0; tileY < 16; tileY++)
	{
		for (BYTE tileX = 0; tileX < 16; tileX++)
		{
			// this is a 32 * 16 "tile" layout. (one tile = 8x8 bits)
			WORD offset = tileY * 256 + tileX * 16;

			for (BYTE row = 0; row < 8; row++)
			{
				BYTE tileLSB = ReadPPU(i * 0x1000 + offset + row + 0);
				BYTE tileMSB = ReadPPU(i * 0x1000 + offset + row + 8);

				for (BYTE col = 0; col < 8; col++)
				{
					BYTE pixel = (tileLSB & 0x01) + (tileMSB & 0x01);
					tileLSB >>= 1;
					tileMSB >>= 1;

					SDL_Color c = PatternPixelScreenColour(palette, pixel);
					SDL_SetRenderDrawColor(m_pRenderer, c.r, c.g, c.b, 255);
					SDL_RenderDrawPoint(m_pRenderer,
						tileX * 8 + (7 - col),
						tileY * 8 + row
						);
				}
			}
		}
	}

	SDL_SetRenderTarget(m_pRenderer, prevRenderTarget);
	return m_pTexPatternTables[i];
}
