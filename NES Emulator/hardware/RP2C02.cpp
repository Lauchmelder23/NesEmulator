#include "RP2C02.hpp"

#include <memory>
#include <iostream>

RP2C02::RP2C02() :
	m_pCartridge(nullptr), m_pNameTables(nullptr), m_pPaletteTable(nullptr), m_nScanline(0), m_nCycle(0),
	m_pTexNameTables(new SDL_Texture*[2] { nullptr, nullptr }), 
	m_pTexPatternTables(new SDL_Texture* [2]{ nullptr, nullptr })
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

	m_pNameTables = new BYTE*[2]{ new BYTE[1024], new BYTE[1024] };

	m_pPaletteTable = new BYTE[32];
}

RP2C02::~RP2C02()
{
	delete[] m_pPaletteTable;
	m_pPaletteTable = nullptr;

	delete[] m_pNameTables[1];
	m_pNameTables[1] = nullptr;

	delete[] m_pNameTables[0];
	m_pNameTables[0] = nullptr;

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

	switch (address)
	{
	case 0x0000:	// PPUCTRL
		break;

	case 0x0001:	// PPUMASK
		break;

	case 0x0002:	// PPUSTATUS
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
		m_nPPUBuffer = ReadPPU(m_oVramRegister.Raw);

		// EXCEPT the palette data :)
		if (m_oVramRegister.Raw > 0x3F00)
			data = m_nPPUBuffer;
		m_oVramRegister.Raw += (m_oControl.VRAMAddressIncrement ? 32 : 1);
		break;
	}

	return data;
}

void RP2C02::WriteCPU(WORD address, BYTE value)
{
	switch (address)
	{
	case 0x0000:	// PPUCTRL
		m_oControl.Raw = value;
		m_oTempVramRegister.Nametable = m_oControl.BaseNametableAddress;
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
		if (m_nAddressLatch == 0x00)
		{
			m_oTempVramRegister.CoarseX = (value >> 3);
			m_nFineX = (value & 0x07);
			m_nAddressLatch = 0x01;
		}
		else
		{
			m_oTempVramRegister.CoarseY = (value >> 3);
			m_oTempVramRegister.FineY = (value & 0x07);
			m_nAddressLatch = 0x00;
		}
		break;
	case 0x0006:	// PPUADDR
		if (m_nAddressLatch == 0x00)
		{
			m_oTempVramRegister.Raw = (m_oTempVramRegister.Raw & 0x00FF) | (value << 8);
			m_nAddressLatch = 0x01;
		}
		else
		{
			m_oTempVramRegister.Raw = (m_oTempVramRegister.Raw & 0xFF00) | value;
			m_nAddressLatch = 0x00;

			m_oVramRegister = m_oTempVramRegister;
		}
		break;

	case 0x0007:	// PPUDATA
		WritePPU(m_oVramRegister.Raw, value);
		m_oVramRegister.Raw += (m_oControl.VRAMAddressIncrement ? 32 : 1);
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
		address &= 0x0FFF;
		if (m_pCartridge->UsedMirroring == MIRROR_VERTICAL)
		{
			if (IS_IN_RANGE(address, 0x0000, 0x03FF))	// Top left
				data = m_pNameTables[0][address];
			if (IS_IN_RANGE(address, 0x0400, 0x07FF))	// Top right
				data = m_pNameTables[1][address];
			if (IS_IN_RANGE(address, 0x0800, 0x0BFF))	// Bottom left
				data = m_pNameTables[0][address & 0x03FF];
			if (IS_IN_RANGE(address, 0x0C00, 0x0FFF))	// Bottom right
				data = m_pNameTables[1][address & 0x03FF];
		}
		else if (m_pCartridge->UsedMirroring == MIRROR_HORIZONTAL)
		{
			if (IS_IN_RANGE(address, 0x0000, 0x03FF))	// Top left
				data = m_pNameTables[0][address];
			if (IS_IN_RANGE(address, 0x0400, 0x07FF))	// Top right
				data = m_pNameTables[0][address & 0x03FF];
			if (IS_IN_RANGE(address, 0x0800, 0x0BFF))	// Bottom left
				data = m_pNameTables[1][address];
			if (IS_IN_RANGE(address, 0x0C00, 0x0FFF))	// Bottom right
				data = m_pNameTables[1][address & 0x03FF];
		}
	}

	// Palette information
	else if (IS_IN_RANGE(address, 0x3F00, 0x3FFF))
	{
		address &= 0x001F;

		// Hardcode in the mirroring because I'm lazy
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
		address &= 0x0FFF;
		if (m_pCartridge->UsedMirroring == MIRROR_VERTICAL)
		{
			if (IS_IN_RANGE(address, 0x0000, 0x03FF))	// Top left
				m_pNameTables[0][address] = value;
			if (IS_IN_RANGE(address, 0x0400, 0x07FF))	// Top right
				m_pNameTables[1][address] = value;
			if (IS_IN_RANGE(address, 0x0800, 0x0BFF))	// Bottom left
				m_pNameTables[0][address & 0x03FF] = value;
			if (IS_IN_RANGE(address, 0x0C00, 0x0FFF))	// Bottom right
				m_pNameTables[1][address & 0x03FF] = value;
		}
		else if (m_pCartridge->UsedMirroring == MIRROR_HORIZONTAL)
		{
			if (IS_IN_RANGE(address, 0x0000, 0x03FF))	// Top left
				m_pNameTables[0][address] = value;
			if (IS_IN_RANGE(address, 0x0400, 0x07FF))	// Top right
				m_pNameTables[0][address & 0x03FF] = value;
			if (IS_IN_RANGE(address, 0x0800, 0x0BFF))	// Bottom left
				m_pNameTables[1][address] = value;
			if (IS_IN_RANGE(address, 0x0C00, 0x0FFF))	// Bottom right
				m_pNameTables[1][address & 0x03FF] = value;
		}
	}

	// Palette information
	else if (IS_IN_RANGE(address, 0x3F00, 0x3FFF))
	{
		address &= 0x001F;

		// Hardcode in the mirroring because I'm lazy
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
	if (IS_IN_RANGE(m_nScanline, -1, 239))
	{
		if (m_nScanline == -1 && m_nCycle == 1)
		{
			m_oStatus.VBlank = 0;
			m_oStatus.SpriteOverflow = 0;
			m_oStatus.SpriteZeroHit = 0;
		}

		// TODO: If something breaks, check timing in here
		if (IS_IN_RANGE(m_nCycle, 1, 257) || IS_IN_RANGE(m_nCycle, 321, 336))
		{
			m_oBgShiftRegister.PatternHi <<= 1;
			m_oBgShiftRegister.PatternLo <<= 1;
			m_oBgShiftRegister.AttribHi <<= 1;
			m_oBgShiftRegister.AttribLo <<= 1;

			switch (m_nCycle % 8)
			{
			case 1:
				m_oBgShiftRegister.PatternLo = (m_oBgShiftRegister.PatternLo & 0xFF00) | m_oBgNextTileInfo.LSB;
				m_oBgShiftRegister.PatternHi = (m_oBgShiftRegister.PatternHi & 0xFF00) | m_oBgNextTileInfo.MSB;
				m_oBgShiftRegister.AttribLo = (m_oBgShiftRegister.AttribLo & 0xFF00)   | ((m_oBgNextTileInfo.Attrib & 0b01) ? 0xFF : 0x00);
				m_oBgShiftRegister.AttribHi = (m_oBgShiftRegister.AttribHi & 0xFF00) | ((m_oBgNextTileInfo.Attrib & 0b10) ? 0xFF : 0x00);
				
				m_oBgNextTileInfo.ID = ReadPPU(0x2000 | (m_oVramRegister.Raw & 0x0FFF));
				break;

			case 3:
				m_oBgNextTileInfo.Attrib = ReadPPU(0x23C0 |
					(m_oVramRegister.Raw & 0x0C00) |
					((m_oVramRegister.Raw >> 4) & 0x38) |
					((m_oVramRegister.Raw >> 2) & 0x07)
				);	// Straight from the Wiki

				// Get the final 2 bits needed from the attribute
				if (m_oVramRegister.CoarseX & 0x02)	// If right side
					m_oBgNextTileInfo.Attrib >>= 2;
				if (m_oVramRegister.CoarseY & 0x02) // If bottom half
					m_oBgNextTileInfo.Attrib >>= 4;

				m_oBgNextTileInfo.Attrib &= 0x03;
				break;

			case 5:
				m_oBgNextTileInfo.LSB = ReadPPU(	// Background Table Base Addr + Tile ID + FineY
					(m_oControl.BackgroundPatternTable << 12)
					+ ((WORD)m_oBgNextTileInfo.ID << 4)
					+ (m_oVramRegister.FineY)
				);
				break;

			case 7:
				m_oBgNextTileInfo.MSB = ReadPPU(	// Background Table Base Addr + Tile ID + FineY + 8
					(m_oControl.BackgroundPatternTable << 12)
					+ ((WORD)m_oBgNextTileInfo.ID << 4)
					+ (m_oVramRegister.FineY) + 8
				);
				break;

			case 0:	// Straight from the Wiki
				if (m_oMask.ShowBackground)
				{
					if ((m_oVramRegister.Raw & 0x001F) == 31)	// if coarse X == 31
					{
						m_oVramRegister.Raw &= ~0x001F;			// coarse X = 0
						m_oVramRegister.Raw ^= 0x0400;			// switch horizontal nametable
					}
					else
					{
						m_oVramRegister.Raw += 1;				// increment coarse X
					}
				}
				break;
			}
		}

		if (m_nCycle == 256)
		{
			if (m_oMask.ShowBackground)
			{
				if ((m_oVramRegister.Raw & 0x7000) != 0x7000)        // if fine Y < 7
				{
					m_oVramRegister.Raw += 0x1000;                      // increment fine Y
				}
				else
				{
					m_oVramRegister.Raw &= ~0x7000;                     // fine Y = 0
					int y = (m_oVramRegister.Raw & 0x03E0) >> 5;        // let y = coarse Y
					if (y == 29)
					{
						y = 0;                          // coarse Y = 0
						m_oVramRegister.Raw ^= 0x0800;                    // switch vertical nametable
					}
					else if (y == 31)
					{
						y = 0;                          // coarse Y = 0, nametable not switched
					}
					else
					{
						// TODO: Coarse Y doesnt get incremented
						y += 1;                         // increment coarse Y
						m_oVramRegister.Raw = (m_oVramRegister.Raw & ~0x03E0) | (y << 5);     // put coarse Y back into v
					}
				}
			}
		}
		
		if (m_nCycle == 257)
		{
			if (m_oMask.ShowBackground)
			{
				m_oVramRegister.Nametable = (m_oVramRegister.Nametable & 0x02) | (m_oTempVramRegister.Nametable & 0x01);
				m_oVramRegister.CoarseX = m_oTempVramRegister.CoarseX;
			}
		}

		if (m_nScanline == -1 && (m_nCycle, 280, 304))
		{
			if (m_oMask.ShowBackground)
			{
				m_oVramRegister.Nametable = (m_oVramRegister.Nametable & 0x01) | (m_oTempVramRegister.Nametable & 0x02);
				m_oVramRegister.CoarseY = m_oTempVramRegister.CoarseY;
				m_oVramRegister.FineY = m_oTempVramRegister.FineY;
			}
		}
	}

	

	if (m_nScanline == 241 && m_nCycle == 1)
	{
		m_oStatus.VBlank = 1;
		if (m_oControl.GenerateNMI)
			isThrowingInterrupt = true;
	}


	// FINALLY!

	BYTE backgroundPixel = 0x00;
	BYTE backgroundAttrib = 0x00;
	if (m_oMask.ShowBackground)
	{
		BYTE pxlLo = (m_oBgShiftRegister.PatternLo & (0x8000 >> m_nFineX)) > 0;
		BYTE pxlHi = (m_oBgShiftRegister.PatternHi & (0x8000 >> m_nFineX)) > 0;	
		backgroundPixel = (pxlHi << 1) | pxlLo;

		BYTE palLo = (m_oBgShiftRegister.AttribLo & (0x8000 >> m_nFineX)) > 0;
		BYTE palHi = (m_oBgShiftRegister.AttribHi & (0x8000 >> m_nFineX)) > 0;
		backgroundAttrib = (palHi << 1) | palLo;
	}

	// Render the pixel
	SDL_Color c = PatternPixelScreenColour(backgroundAttrib, backgroundPixel);
	SDL_SetRenderDrawColor(m_pRenderer, c.r, c.g, c.b, 255);
	SDL_RenderDrawPoint(m_pRenderer, m_nCycle, m_nScanline);


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
