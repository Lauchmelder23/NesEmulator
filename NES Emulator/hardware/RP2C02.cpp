#include "RP2C02.hpp"

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
}

RP2C02::~RP2C02()
{
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
		break;
	case 0x0003:	// OAMADDR
		break;
	case 0x0004:	// OAMDATA
		break;
	case 0x0005:	// PPUSCROLL
		break;
	case 0x0006:	// PPUADDR
		break;
	case 0x0007:	// PPUDATA
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
		break;
	case 0x0001:	// PPUMASK
		break;
	case 0x0002:	// PPUSTATUS
		break;
	case 0x0003:	// OAMADDR
		break;
	case 0x0004:	// OAMDATA
		break;
	case 0x0005:	// PPUSCROLL
		break;
	case 0x0006:	// PPUADDR
		break;
	case 0x0007:	// PPUDATA
		break;
	}
}

BYTE RP2C02::ReadPPU(WORD address, bool readonly)
{
	BYTE data = 0x00;
	address &= 0x3FFF;

	return data;
}

void RP2C02::WritePPU(WORD address, BYTE value)
{
	address &= 0x3FFF;
}

void RP2C02::InsertCartridge(const Cartridge* cartridge)
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

SDL_Texture* RP2C02::GetScreen()
{
	return m_pScreen;
}

SDL_Texture* RP2C02::GetNameTable(BYTE i)
{
	return m_pTexNameTables[i];
}

SDL_Texture* RP2C02::GetPatternTable(BYTE i)
{
	return m_pTexPatternTables[i];
}
