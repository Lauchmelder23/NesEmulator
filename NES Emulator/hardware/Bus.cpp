#include "Bus.hpp"
#include <memory>
#include <string.h>
#include "../util.hpp"

Bus::Bus() :
	m_pRAM(nullptr)
{
	// Initialize and clear RAM
	m_pRAM = new BYTE[0x10000]; //64 KB of RAM
	std::memset(m_pRAM, 0, 0x10000);

	// Connect CPU to BUS
	m_oCPU.ConnectBus(this);
	m_oCPU.Reset();
}

Bus::~Bus()
{
	// Clean up RAM
	delete[] m_pRAM;
	m_pRAM = nullptr;
}

void Bus::Write(WORD address, BYTE value)
{
	if(IS_IN_RANGE(address, 0x0000, 0xFFFF))
		m_pRAM[address] = value;
}

BYTE Bus::Read(WORD address, bool readOnly /*= false*/)
{
	if (IS_IN_RANGE(address, 0x0000, 0xFFFF))
		return m_pRAM[address];

	return 0x00;
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
			map << HEX("", Read(ptr + i), 2) << " ";
			if (!((i + 1) % 8))
				map << "  ";
		}

		// Print ascii output
		map << "|";
		for (int i = 0; i < 0x10; i++)
		{
			BYTE b = Read(ptr + i);
			if (std::isprint(b))
				map << b;
			else
				map << ".";
		}
		map << "|\n";
	}

	return map.str();
}
