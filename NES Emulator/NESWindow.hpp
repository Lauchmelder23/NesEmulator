#pragma once

#include <SDL.h>
#include <SDLF.hpp>
#include <sstream>

#include "Bus.hpp"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define SCALE 3

class NESWindow : public sf::IWindow
{
public:
	NESWindow() :
		IWindow(sf::Vec2u(SCREEN_WIDTH, SCREEN_HEIGHT) * SCALE + sf::Vec2u(500, 100), sf::Vec2i(100, 100), "NESemu", 
			SDL_WINDOW_SHOWN, SDL_RENDERER_TARGETTEXTURE),
		m_pTexture(nullptr)
	{

	}

private:
	virtual bool OnCreate() override
	{
		m_pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGB888, 
			SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

		if (m_pTexture == nullptr)
			return false;

		m_pScreen = new SDL_Rect;
		m_pScreen->x = 10;
		m_pScreen->y = 10;
		m_pScreen->w = SCREEN_WIDTH * SCALE;
		m_pScreen->h = SCREEN_HEIGHT * SCALE;

		std::stringstream ss;
		ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
		WORD offset = 0x8000;
		while (!ss.eof())
		{
			std::string b;
			ss >> b;
			m_oNes.Write(offset++, static_cast<BYTE>(std::stoul(b, nullptr, 16)));
		}

		m_mapDisassemble = m_oNes.m_oCPU.Disassemble(0x8000, 0x9000);

		m_oNes.Write(0xFFFC, 0x00);
		m_oNes.Write(0xFFFD, 0x80);

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
					std::cout << disas->second << "\t\t"
					<< "A=" << m_oNes.m_oCPU.m_uAcc << ", "
					<< "X=" << m_oNes.m_oCPU.m_uX << ", "
					<< "Y=" << m_oNes.m_oCPU.m_uY << ", "
					<< "S=" << m_oNes.m_oCPU.m_uSP << ", "
					<< "F=" << m_oNes.m_oCPU.m_oStatus.Raw << std::endl;

				do
				{
					m_oNes.m_oCPU.Tick();
				} while (!m_oNes.m_oCPU.Done());
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
		SDL_SetRenderDrawColor(m_pRenderer, 30, 0, 30, 255);
		SDL_RenderClear(m_pRenderer);
		// Set Render target to the texture
		SDL_SetRenderTarget(m_pRenderer, m_pTexture);

		// Clear the texture
		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(m_pRenderer);

		// Set render target back to window
		SDL_SetRenderTarget(m_pRenderer, NULL);

		// Copy texture to window
		SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, m_pScreen);
	}

	virtual void OnClose() override 
	{
		SDL_DestroyTexture(m_pTexture);
	}

private:
	SDL_Texture* m_pTexture;
	SDL_Rect* m_pScreen;
	Bus m_oNes;

	std::map<WORD, std::string> m_mapDisassemble;
};