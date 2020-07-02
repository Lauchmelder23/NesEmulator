#pragma once

#include <SDL.h>
#include <SDLF.hpp>
#include <sstream>

#include "Bus.hpp"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define SCALE 3

#define HEIGHT_PER_LINE 20

class NESWindow : public sf::IWindow
{
public:
	NESWindow(const char* filename) :
		IWindow(sf::Vec2u(SCREEN_WIDTH, SCREEN_HEIGHT)* SCALE, sf::Vec2i(100, 100), "NESemu",
			SDL_WINDOW_SHOWN, SDL_RENDERER_TARGETTEXTURE),
		m_pTexture(nullptr), m_pCartridge(nullptr), m_pFilename(filename), m_oNes(this)
	{
		
	}

	SDL_Renderer* GetRenderer() { return m_pRenderer; }

private:
	virtual bool OnCreate() override;
	virtual bool OnEvent(const SDL_Event& event) override;
	virtual bool OnUpdate(double frametime) override;
	virtual void OnRender(SDL_Renderer* renderer) override;
	virtual void OnClose() override;

	void PrintCurrentInstruction();

private:
	SDL_Texture* m_pTexture;

	Bus m_oNes;
	Cartridge* m_pCartridge;

	const char* m_pFilename;

	std::map<WORD, std::string> m_mapDisassemble;
};