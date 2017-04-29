#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <map>
#include <stdexcept>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "point.h"
#include "version.h"

class Game {
public:
	Game(std::string aName, Version version);
	
	//Pushes an SDL_QUIT event to the event list
	static void quit();
private:
	//Loads the external settings stored in settings.txt
	void loadSettings();
	
	//Initializes SDL subsystems
	void init();
	
	//Timer stuff
	SDL_TimerID id;
	//Old C code
	static Uint32 timer(Uint32 ms, void* param);
	
	//Main loop of the program
	void mainLoop();
	
	//Renders everything on the game (Terrain, Items, etc)
	void renderGame();
	
	//Closes SDL subsystems and deletes variables
	void cleanup();
	
	//Redirects the clog to a file
	std::ofstream log;
	
	bool generateDefaultSettings;
};
