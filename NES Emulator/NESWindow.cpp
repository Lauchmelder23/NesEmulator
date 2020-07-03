#include "NESWindow.hpp"
#include <iostream>

#include "editme.hpp"

bool NESWindow::OnCreate()
{
	m_pCartridge = new Cartridge(m_pFilename);
	m_oNes.InsertCartridge(m_pCartridge);

	m_mapDisassemble = m_oNes.m_oCPU.Disassemble(0x0000, 0xFFFF);

	// Boot the NES
	m_oNes.m_oPPU.Initialize(m_pRenderer);
	m_oNes.m_oCPU.Reset();

	return true;
}

bool NESWindow::OnEvent(const SDL_Event& event)
{
	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
		{
			m_bEmulate = !m_bEmulate;
		}

		if(!m_bEmulate)
		{
			if (event.key.keysym.scancode == SDL_SCANCODE_C && !m_oNes.m_oCPU.Halted())
			{
				do { m_oNes.Clock(); } while (!m_oNes.m_oCPU.Done());
				do { m_oNes.Clock(); } while (m_oNes.m_oCPU.Done());

#ifdef PRINT_INSTRUCTIONS
				PrintCurrentInstruction();
#endif // PRINT_INSTRUCTIONS
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_F)
			{
				do { m_oNes.Clock(); } while (!m_oNes.m_oPPU.isFrameComplete);
				do { m_oNes.Clock(); } while (!m_oNes.m_oCPU.Done());
				m_oNes.m_oPPU.isFrameComplete = false;
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_R)
				m_oNes.m_oCPU.Reset();


			if (event.key.keysym.scancode == SDL_SCANCODE_I)
				m_oNes.m_oCPU.IRQ();


			if (event.key.keysym.scancode == SDL_SCANCODE_N)
				m_oNes.m_oCPU.NMI();
		}
		
	}

	return true;
}

bool NESWindow::OnUpdate(double frametime)
{
	if (m_bEmulate)
	{
		do { m_oNes.Clock(); } while (!m_oNes.m_oPPU.isFrameComplete);
		do { m_oNes.Clock(); } while (!m_oNes.m_oCPU.Done());
		m_oNes.m_oPPU.isFrameComplete = false;
	}

	return true;
}

void NESWindow::OnRender(SDL_Renderer* renderer)
{
#ifndef RENDER_MEMORY
	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 100, 0, 100, 255);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, m_oNes.m_oPPU.GetScreen(), NULL, new SDL_Rect{ 10, 10, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2 });
	SDL_SetRenderTarget(renderer, m_oNes.m_oPPU.GetScreen());
#endif // RENDER_MEMORY
}

void NESWindow::OnClose()
{
	
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
		<< "F=" << m_oNes.m_oCPU.m_oStatus.AsString() << " [" << HEX("", m_oNes.m_oCPU.m_oStatus.Raw, 2) << "] ";

	ss << "(" << std::dec << m_oNes.m_oPPU.GetScanline() << ", " << m_oNes.m_oPPU.GetCycle() - 1 << ")" << std::endl;

	std::cout << ss.str();
}
