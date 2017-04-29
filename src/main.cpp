#include <iostream>
#include <string>

#include "game.h"
#include "version.h"

//2016-11-28

const std::string APPNAME = "Fall of Dyrangor";
const Version VERSION = Version("1.0.0.1");

int main ( int argc, char* args[] ){
	//Game is not extern anymore
	Game * game;
	
	//Init
	game = new Game(APPNAME, VERSION);
	
	//Not needed possibly
	delete game;
	
	return 0;
}
