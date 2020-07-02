// NES Emulator.cpp : Defines the entry point for the application.
//
#include <iostream>
#include "NESWindow.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Incorrect amount of arguments." << std::endl
			<< "Usage: nesemu <NES ROM File>" << std::endl;
		return 1;
	}

	SDL_Init(SDL_INIT_VIDEO);

	NESWindow window(argv[1]);

	window.Launch(true);

	while (window.IsOpen())
	{
		// :)
	}

	window.Stop();

	return 0;
}
