#pragma once

#include <SDL.h>
#include <SDLF.hpp>
#include <sstream>

#include "Bus.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 800
#define SCALE 1

#define HEIGHT_PER_LINE 20

class NESWindow : public sf::IWindow
{
public:
	NESWindow(const char* filename) :
		IWindow(sf::Vec2u(SCREEN_WIDTH, SCREEN_HEIGHT)* SCALE, sf::Vec2i(100, 100), "NESemu",
			SDL_WINDOW_SHOWN, SDL_RENDERER_TARGETTEXTURE),
		m_pTexture(nullptr), m_pScreen(nullptr), m_pCartridge(nullptr), m_pFilename(filename)
	{

	}

private:
	bool OnCreate() override;
	bool OnEvent(const SDL_Event& event);
	bool OnUpdate(double frametime) override;
	void OnRender(SDL_Renderer* renderer) override;
	void OnClose() override;

	void PrintCurrentInstruction();

private:
	SDL_Texture* m_pTexture;
	SDL_Rect* m_pScreen;

	Bus m_oNes;
	Cartridge* m_pCartridge;

	const char* m_pFilename;

	std::map<WORD, std::string> m_mapDisassemble;
};