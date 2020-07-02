#include "Bus.hpp"
#include <memory>
#include <string.h>

#include "../util.hpp"
#include "../editme.hpp"
#include "../NESWindow.hpp"

Bus::Bus(NESWindow* parentWindow) :
	m_pCPURAM(nullptr), m_pParentWindow(parentWindow)
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
	if (IS_IN_RANGE(address, 0x0000, 0x1FFF))
	{
		m_pCPURAM[address & 0x7FF] = data;
#ifdef RENDER_MEMORY
		RenderData(address & 0x7FF, data);
#endif // RENDER_MEMORY
	}

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

void Bus::RenderData(WORD address, BYTE data)
{
	SDL_Rect r = { (address % (SCREEN_WIDTH / SCALE - 1)) * (SCALE * 6), 
		((address / (SCREEN_WIDTH / SCALE))) * SCALE * 6, 
		SCALE * 6, 
		SCALE * 6 };
	SDL_SetRenderDrawColor(m_pParentWindow->GetRenderer(), data, data, data, 255);
	SDL_RenderFillRect(m_pParentWindow->GetRenderer(), &r);
}
