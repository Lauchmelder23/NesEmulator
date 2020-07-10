#include "NESWindow.hpp"
#include <iostream>

#include "editme.hpp"

bool NESWindow::OnCreate()
{
	try 
	{
		m_pCartridge = std::make_shared<Cartridge>(m_pFilename);
	}
	catch (std::string e)
	{
		std::cerr << e << std::endl;
		return false;
	}

	m_oNes.InsertCartridge(m_pCartridge);

#ifdef PRINT_INSTRUCTIONS
	m_mapDisassemble = m_oNes.m_oCPU.Disassemble(0x0000, 0xFFFF);
#endif

#ifdef LOG_INSTRUCTIONS
	file.open("dump.log");
#endif

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

		if (event.key.keysym.scancode == SDL_SCANCODE_P)
		{
			(++m_nSelectedPalette) &= 0x07;
		}

		if(!m_bEmulate)
		{
			if (event.key.keysym.scancode == SDL_SCANCODE_C && !m_oNes.m_oCPU.Halted())
			{
				do { m_oNes.Clock(); } while (!m_oNes.m_oCPU.Done());
				do { m_oNes.Clock(); } while (m_oNes.m_oCPU.Done());
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
	m_oNes.m_arrController[0] = 0x00;
	const BYTE* state = SDL_GetKeyboardState(NULL);
	m_oNes.m_arrController[0] |= state[SDL_SCANCODE_COMMA] << 7;	// A
	m_oNes.m_arrController[0] |= state[SDL_SCANCODE_PERIOD] << 6;	// B
	m_oNes.m_arrController[0] |= state[SDL_SCANCODE_RETURN] << 5;	// Select
	m_oNes.m_arrController[0] |= state[SDL_SCANCODE_RSHIFT] << 4;	// Start
	m_oNes.m_arrController[0] |= state[SDL_SCANCODE_W] << 3;	// Up
	m_oNes.m_arrController[0] |= state[SDL_SCANCODE_S] << 2;	// Down
	m_oNes.m_arrController[0] |= state[SDL_SCANCODE_A] << 1;	// Left
	m_oNes.m_arrController[0] |= state[SDL_SCANCODE_D] << 0;	// Right

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
	SDL_SetRenderTarget(renderer, NULL);

	// Clear screen (is this necessary?)
	SDL_SetRenderDrawColor(renderer, 100, 0, 100, 255);
	SDL_RenderClear(renderer);

	// The main screen
	SDL_RenderCopy(renderer, m_oNes.m_oPPU.GetScreen(), NULL, new SDL_Rect{ 10, 10, SCREEN_WIDTH * (SCALE - 1), SCREEN_HEIGHT * (SCALE - 1) });
	
	// The pattern memory
	RenderPatternTables();
	RenderPalettes();

	SDL_SetRenderTarget(renderer, m_oNes.m_oPPU.GetScreen());
}

void NESWindow::OnClose()
{
#ifdef LOG_INSTRUCTIONS
	if (file.is_open())
		file.close();
#endif
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

#ifdef LOG_INSTRUCTIONS
	file << ss.str();
#endif
}

void NESWindow::RenderPatternTables()
{
	SDL_RenderCopy(m_pRenderer, m_oNes.m_oPPU.GetPatternTable(0, m_nSelectedPalette), NULL,
		new SDL_Rect{ 10, 10 + SCREEN_HEIGHT * (SCALE - 1) + 20, 128 * (SCALE - 1), 128 * (SCALE - 1) });

	SDL_RenderCopy(m_pRenderer, m_oNes.m_oPPU.GetPatternTable(1, m_nSelectedPalette), NULL,
		new SDL_Rect{ 10 + 128 * (SCALE - 1) + 20, 10 + SCREEN_HEIGHT * (SCALE - 1) + 20, 128 * (SCALE - 1), 128 * (SCALE - 1) });
}

void NESWindow::RenderPalettes()
{
	SDL_Color c;
	SDL_Rect rect;
	rect.w = SCALE * 10;
	rect.h = SCALE * 10;
	for (BYTE palette = 0; palette < 0x08; palette++)
	{
		rect.y = 10 + (rect.h + 10) * palette;
		for (BYTE color = 0; color < 0x04; color++)
		{
			rect.x = 10 + SCREEN_WIDTH * (SCALE - 1) + 20 + (rect.w * color);
			rect.x += (palette == m_nSelectedPalette) ? rect.w / 2 : 0;
			c = m_oNes.m_oPPU.PatternPixelScreenColour(palette, color);
			SDL_SetRenderDrawColor(m_pRenderer, c.r, c.g, c.b, 255);
			SDL_RenderFillRect(m_pRenderer, &rect);
		}
	}
}
