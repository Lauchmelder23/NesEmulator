#pragma once

#include <SDL.h>
#include <SDLF.hpp>

#include "Bus.hpp"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define SCALE 3

class NESWindow : public sf::IWindow
{
public:
	NESWindow() :
		IWindow(sf::Vec2u(SCREEN_WIDTH, SCREEN_HEIGHT) * SCALE, sf::Vec2i(100, 100), "NESemu", 
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

		Bus bus;

		return true;
	}

	virtual void OnRender(SDL_Renderer* renderer) override
	{
		// Set Render target to the texture
		SDL_SetRenderTarget(m_pRenderer, m_pTexture);

		// Clear the texture
		SDL_SetRenderDrawColor(m_pRenderer, 80, 80, 255, 255);
		SDL_RenderClear(m_pRenderer);

		// Set render target back to window
		SDL_SetRenderTarget(m_pRenderer, NULL);

		// Copy texture to window
		SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, NULL);
	}

	virtual void OnClose() override 
	{
		SDL_DestroyTexture(m_pTexture);
	}

private:
	SDL_Texture* m_pTexture;
};