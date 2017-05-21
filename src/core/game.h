#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <map>
#include <stdexcept>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <thread>
#include <ctime>

#include "../util/point.h"
#include "../util/version.h"

/*!
 * @author kovlev
 */


/*!
 * @class Game the base game
 * Calls all initialization functions
 * Runs the mainloop that deals with timing events and rendering
 * Cleans up the game by freeing all memory and deleting variables
 */
class Game {
public:
	Game(std::string aName, Version version);
	
	//Pushes an SDL_QUIT event to the event list
	static void quit();
	
	//Renders the screen a bunch of times
	static void checkPerformance();
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
	
	//Do we ned to generate settings or there is an existing file? 
	bool generateDefaultSettings;
	
	//Initializer boundaries
	const static int MIN_WIDTH;
	const static int MAX_WIDTH;
	const static int MIN_HEIGHT;
	const static int MAX_HEIGHT;
	const static int MIN_FPS;
	const static int MAX_FPS;
	
	const static int TESTCOUNT;
};
