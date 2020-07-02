#include "NESWindow.hpp"
#include <iostream>

#include "editme.hpp"

bool NESWindow::OnCreate()
{
	// Create the NES screen
	m_pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGB888,
		SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (m_pTexture == nullptr)
		return false;

	m_pCartridge = new Cartridge(m_pFilename);
	m_oNes.InsertCartridge(m_pCartridge);

	m_mapDisassemble = m_oNes.m_oCPU.Disassemble(0x0000, 0xFFFF);

	// Boot the NES
	m_oNes.m_oCPU.Reset();

	return true;
}

bool NESWindow::OnEvent(const SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.scancode == SDL_SCANCODE_R)
			m_oNes.m_oCPU.Reset();


		if (event.key.keysym.scancode == SDL_SCANCODE_I)
			m_oNes.m_oCPU.IRQ();


		if (event.key.keysym.scancode == SDL_SCANCODE_N)
			m_oNes.m_oCPU.NMI();
	}

	return true;
}

bool NESWindow::OnUpdate(double frametime)
{
	try {
		do
		{
			m_oNes.m_oCPU.Tick();
		} while (!m_oNes.m_oCPU.Done());
	}
	catch (std::string s)
	{
		std::cerr << s << std::endl;
		return false;
	}

#ifdef PRINT_INSTRUCTIONS
	PrintCurrentInstruction();
#endif // PRINT_INSTRUCTIONS

	return true;
}

void NESWindow::OnRender(SDL_Renderer* renderer)
{
#ifndef RENDER_MEMORY
	SDL_RenderCopy(renderer, m_pTexture, NULL, NULL);
#endif // RENDER_MEMORY
}

void NESWindow::OnClose()
{
	SDL_DestroyTexture(m_pTexture);
}

void NESWindow::PrintCurrentInstruction()
{
	std::stringstream ss;
	ss << "(" << std::dec << m_oNes.m_oCPU.GetCycles() << ") " 
		<< HEX("$", m_oNes.m_oCPU.m_uPC, 4) << "  ";
	if (!m_oNes.m_oCPU.Halted())
	{
		auto disas = m_mapDisassemble.find(m_oNes.m_oCPU.m_uPC);
		if (disas != m_mapDisassemble.end())
			ss << disas->second << "\t\t";
	}
	else {
		ss << HEX("", m_oNes.ReadCPU(m_oNes.m_oCPU.m_uPC), 2) 
			<< " JAM\t\t";
	}
	ss
		<< "A=" << (WORD)m_oNes.m_oCPU.m_uAcc << ", "
		<< "X=" << (WORD)m_oNes.m_oCPU.m_uX << ", "
		<< "Y=" << (WORD)m_oNes.m_oCPU.m_uY << ", "
		<< "S=" << (WORD)m_oNes.m_oCPU.m_uSP << ", "
		<< "F=" << m_oNes.m_oCPU.m_oStatus.AsString() << " (" << HEX("", m_oNes.m_oCPU.m_oStatus.Raw, 2) << ")" << std::endl;

	std::cout << ss.str();
}
