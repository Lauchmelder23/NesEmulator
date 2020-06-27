#include "RP2C02.hpp"

RP2C02::RP2C02()
{
}

RP2C02::~RP2C02()
{
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
