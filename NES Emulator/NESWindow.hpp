#pragma once

#include <SDL.h>
#include <SDLF.hpp>
#include <sstream>

#include "../editme.hpp"

#ifdef LOG_INSTRUCTIONS
#include <fstream>
#endif

#include "Bus.hpp"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define SCALE 3

#define HEIGHT_PER_LINE 20

class NESWindow : public sf::IWindow
{
public:
	NESWindow(const char* filename) :
		IWindow(sf::Vec2u(SCREEN_WIDTH, SCREEN_HEIGHT)* SCALE + sf::Vec2u(100, 100), sf::Vec2i(100, 100), "NESemu",
			SDL_WINDOW_SHOWN, SDL_RENDERER_TARGETTEXTURE),
		m_pCartridge(nullptr), m_pFilename(filename), m_oNes(this)
	{
		
	}

	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	void PrintCurrentInstruction();

private:
	virtual bool OnCreate() override;
	virtual bool OnEvent(const SDL_Event& event) override;
	virtual bool OnUpdate(double frametime) override;
	virtual void OnRender(SDL_Renderer* renderer) override;
	virtual void OnClose() override;

	void RenderPatternTables();
	void RenderPalettes();

private:
	Bus m_oNes;
	Cartridge* m_pCartridge;

	BYTE m_nSelectedPalette = 0x00;
	bool m_bEmulate = false;
	const char* m_pFilename;

	std::map<WORD, std::string> m_mapDisassemble;
#ifdef LOG_INSTRUCTIONS
	std::ofstream file;
#endif
};