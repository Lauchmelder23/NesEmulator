#include "Bus.hpp"
#include <memory>
#include <string.h>
#include "../util.hpp"

Bus::Bus() :
	m_pCPURAM(nullptr)
{
	// Initialize and clear RAM
	m_pCPURAM = new BYTE[0x800]; // The CPU can access 2KB of RAM (actually 8KB, but the 2KB are mirrored 3 times)

	// Connect CPU to BUS
	m_oCPU.ConnectBus(this);
}

Bus::~Bus()
{
	// Clean up RAM
	delete[] m_pCPURAM;
	m_pCPURAM = nullptr;
}

void Bus::WriteCPU(WORD address, BYTE data)
{
	if (m_pCartridge->WriteCPU(address, data))
	{

	}
	// Between 0x0000 and 0x1FFF we are accessing the CPU's RAM
	if(IS_IN_RANGE(address, 0x0000, 0x1FFF))
		m_pCPURAM[address & 0x7FF] = data;

	// Between 0x2000 and 0x3FFF we are accessing PPU registers
	else if (IS_IN_RANGE(address, 0x2000, 0x3FFF))
		m_oPPU.WriteCPU(address & 0x7, data);
}

BYTE Bus::ReadCPU(WORD address, bool readOnly /*= false*/)
{
	BYTE data = 0x00;

	if (m_pCartridge->ReadCPU(address, data))
	{

	}
	else if (IS_IN_RANGE(address, 0x0000, 0x1FFF))
		data = m_pCPURAM[address & 0x7FF];

	else if (IS_IN_RANGE(address, 0x2000, 0x3FFF))
		data = m_oPPU.ReadCPU(address & 0x7, readOnly);

	return data;
}

void Bus::InsertCartridge(Cartridge* cartridge)
{
	m_pCartridge = cartridge;
	m_oPPU.InsertCartridge(cartridge);
}

void Bus::Reset()
{
	m_oCPU.Reset();
	m_uClockCounter = 0;
}

void Bus::Clock()
{
}

std::string Bus::GetMemoryMap(WORD begin, WORD end)
{
	/*
	Layout:

	$0000   00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00   |................|
	$0010   00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00   |................|
	. 
	.
	.
	*/

	begin -= (begin % 16);
	end += (16 - (end % 16));

	std::stringstream map;
	for (WORD ptr = begin; ptr < end; ptr += 0x10)
	{
		// Print address in front
		map << HEX("$", ptr, 4) << "   ";

		// Print hex values
		for (int i = 0; i < 0x10; i++)
		{
			map << HEX("", ReadCPU(ptr + i), 2) << " ";
			if (!((i + 1) % 8))
				map << "  ";
		}

		// Print ascii output
		map << "|";
		for (int i = 0; i < 0x10; i++)
		{
			BYTE b = ReadCPU(ptr + i);
			if (std::isprint(b))
				map << b;
			else
				map << ".";
		}
		map << "|\n";
	}

	return map.str();
}
