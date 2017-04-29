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
#include <thread>

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
	SDL_TimerID frameID;
	SDL_TimerID tickID;
	//Old C code
	static uint32_t timer(uint32_t ms, void* param);
	
	//Main loop of the program
	void mainLoop();
	
	//Renders everything on the game (Terrain, Items, etc)
	void renderGame();
	
	//Closes SDL subsystems and deletes variables
	void cleanup();
	
	//Redirects the clog to a file
	std::ofstream log;
	
	bool generateDefaultSettings;
	
	//Threading magic
	std::thread t0;
	std::thread t1;
};
