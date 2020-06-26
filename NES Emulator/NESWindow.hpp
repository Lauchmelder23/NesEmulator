#pragma once

#include <SDL.h>
#include <SDLF.hpp>
#include <SDL_ttf.h>
#include <sstream>

#include "Bus.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 800
#define SCALE 1

#define HEIGHT_PER_LINE 20

class NESWindow : public sf::IWindow
{
public:
	NESWindow() :
		IWindow(sf::Vec2u(SCREEN_WIDTH, SCREEN_HEIGHT)* SCALE, sf::Vec2i(100, 100), "NESemu",
			SDL_WINDOW_SHOWN, SDL_RENDERER_TARGETTEXTURE),
		m_pTexture(nullptr), m_pMemoryMap(nullptr)
	{

	}

private:
	virtual bool OnCreate() override
	{
		TTF_Init();

		// Create the NES screen
		m_pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGB888, 
			SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

		if (m_pTexture == nullptr)
			return false;

		m_pScreen = new SDL_Rect;
		m_pScreen->x = 10;
		m_pScreen->y = 10;
		m_pScreen->w = SCREEN_WIDTH * SCALE;
		m_pScreen->h = SCREEN_HEIGHT * SCALE;

		m_pOutRect = new SDL_Rect;
		m_pOutRect->x = 0;
		m_pOutRect->y = 0;
		m_pOutRect->w = SCREEN_WIDTH * SCALE;
		m_pOutRect->h = 0;

		// Create font for memory map
		m_pFont = TTF_OpenFont("PressStart2P.ttf", 12);
		if (m_pFont == nullptr)
		{
			std::cerr << "Failed to laod font" << std::endl;
			return false;
		}

		// Feed in some dummy byte code for testing
		// TODO: Remove
		std::stringstream ss;
		ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
		WORD offset = 0x8000;
		while (!ss.eof())
		{
			std::string b;
			ss >> b;
			m_oNes.Write(offset++, static_cast<BYTE>(std::stoul(b, nullptr, 16)));
		}

		// Disassemble the program
		m_mapDisassemble = m_oNes.m_oCPU.Disassemble(0x8000, 0x9000);

		// Set reset vectors
		m_oNes.Write(0xFFFC, 0x00);
		m_oNes.Write(0xFFFD, 0x80);

		// Boot the NES
		m_oNes.m_oCPU.Reset();

		return true;
	}





	virtual bool OnEvent(const SDL_Event& event)
	{
		if (event.type == SDL_KEYDOWN)
		{
			if(event.key.keysym.scancode == SDL_SCANCODE_SPACE)
			{
				auto disas = m_mapDisassemble.find(m_oNes.m_oCPU.m_uPC);
				if (disas != m_mapDisassemble.end())
					std::cout << "(" << m_oNes.m_oCPU.GetCycles() << ") "
					<< disas->second << "\t\t"
					<< "A=" << (WORD)m_oNes.m_oCPU.m_uAcc << ", "
					<< "X=" << (WORD)m_oNes.m_oCPU.m_uX << ", "
					<< "Y=" << (WORD)m_oNes.m_oCPU.m_uY << ", "
					<< "S=" << (WORD)m_oNes.m_oCPU.m_uSP << ", "
					<< "F=" << m_oNes.m_oCPU.m_oStatus.AsString() << std::endl;

				std::string s = m_oNes.GetMemoryMap(0x0000, 0x1000);
				m_pMemoryMapSurface = TTF_RenderText_Blended_Wrapped(
					m_pFont, s.c_str(), { 255, 255, 255 }, SCREEN_WIDTH * SCALE
				);

				if (m_pMemoryMapSurface == nullptr)
				{
					std::cerr << SDL_GetError() << std::endl;
					m_atomWindowOpen = false;
				}

				m_pMemoryMap = SDL_CreateTextureFromSurface(m_pRenderer, m_pMemoryMapSurface);
				
				if (m_pMemoryMap == nullptr)
				{
					std::cerr << SDL_GetError() << std::endl;
					m_atomWindowOpen = false;
				}
				
				m_pOutRect->h = std::count(s.begin(), s.end(), '\n') * HEIGHT_PER_LINE;

				try {
					do
					{
						m_oNes.m_oCPU.Tick();
					} while (!m_oNes.m_oCPU.Done());
				}
				catch (std::string s)
				{
					std::cerr << s << std::endl;
					m_atomWindowOpen = false;
				}
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_R)
				m_oNes.m_oCPU.Reset();


			if (event.key.keysym.scancode == SDL_SCANCODE_I)
				m_oNes.m_oCPU.IRQ();


			if (event.key.keysym.scancode == SDL_SCANCODE_N)
				m_oNes.m_oCPU.NMI();	
		}

		return true;
	}





	virtual void OnRender(SDL_Renderer* renderer) override
	{
		// Set Render target to the texture
		SDL_SetRenderTarget(m_pRenderer, m_pTexture);

		// Clear the texture
		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(m_pRenderer);

		// Render Memory map
		if (m_pMemoryMap != nullptr)
			SDL_RenderCopy(m_pRenderer, m_pMemoryMap, NULL, m_pOutRect);

		// Set render target back to window
		SDL_SetRenderTarget(m_pRenderer, NULL);

		// Copy texture to window
		SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, m_pScreen);
	}

	virtual void OnClose() override 
	{
		SDL_DestroyTexture(m_pTexture);

		TTF_CloseFont(m_pFont);
		TTF_Quit();
	}

private:
	SDL_Texture* m_pTexture;
	SDL_Rect* m_pScreen;

	TTF_Font* m_pFont;
	SDL_Surface* m_pMemoryMapSurface;
	SDL_Texture* m_pMemoryMap;
	SDL_Rect* m_pOutRect;

	Bus m_oNes;

	std::map<WORD, std::string> m_mapDisassemble;
};