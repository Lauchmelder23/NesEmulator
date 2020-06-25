#include "Bus.hpp"
#include <memory>

Bus::Bus() :
	m_pRAM(nullptr)
{
	// Initialize and clear RAM
	m_pRAM = new BYTE[0x10000]; //64 KB of RAM
	memset(m_pRAM, 0, 0x10000);

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
