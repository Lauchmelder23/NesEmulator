// NES Emulator.cpp : Defines the entry point for the application.
//
#include <iostream>
#include "NESWindow.hpp"

#undef main

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	NESWindow window;
	try {
		window.Launch(true);
	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
	}

	while (window.IsOpen())
		1;

	window.Stop();

	return 0;
}
