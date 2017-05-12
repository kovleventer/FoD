#include "game.h"

#include "../map/map.h"
#include "resourcehandler.h"
#include "itemhandler.h"
#include "unithandler.h"
#include "global.h"
#include "../util/exceptions.h"
#include "userinputhandler.h"
#include "../map/camera.h"
#include "npchandler.h"
#include "../util/filesystemhandler.h"
#include "worldobjecthandler.h"
#include "../map/player.h"
#include "../gui/popup.h"

/*!
 * @author kovlev
 */

Game::Game(std::string aName, Version version) {
	generateDefaultSettings = false;
	
	//Passing metadata
	Global::appName = aName + version.toString();
	
	Global::defaultFontSize = 100;
	
	Global::tileSize = 64;
	
	Global::ticks = 64;
	
	//Redirecting the default clog to file
	//https://stackoverflow.com/questions/34618916/moving-stdclog-in-source-to-output-file
	//Gets current time
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::stringstream logFileName;
	logFileName << "log/";
	logFileName << std::put_time(now, "%Y-%m-%d-%H-%M-%S");
	logFileName << ".log";
	log.open(logFileName.str());
	std::clog.rdbuf(log.rdbuf());
	
	try {
		loadSettings();
		
		init();
		
		Global::camera = new Camera();
		
		Global::animationHandler = new AnimationHandler();
	
		Global::resourceHandler = new ResourceHandler();
		Global::resourceHandler->loadAll();
		
		Global::itemHandler = new ItemHandler();
		Global::itemHandler->loadAll();
		
		Global::unitHandler = new UnitHandler();
		
		Global::player = new Player("player", 0, 0);
		
		Global::permaGUI = new PermanentGUI();
		
		Global::guiHandler = new GUIHandler();
		
		//Generate map stuff
		//NOTE this must run after the media has been loaded
		Global::map = new Map();
		Global::minimap->regenerateMinimap();
		
		Global::worldObjectHandler = new WorldObjectHandler();
		Global::worldObjectHandler->loadAll();
		
		//This must run after we loaded the world objects but before we loaded the npcs
		Global::map->createPassabilityMap();
		
		Global::npcHandler = new NPCHandler();
		Global::npcHandler->loadAll();
		
		Global::worldObjectHandler->setOwnershipRelations();
		
		Global::cursor = new Cursor("impassable");
		
		
		//60 fps (or not)
		frameID = SDL_AddTimer(1000 / Global::fps, timer, (char*)1);
		tickID = SDL_AddTimer(1000 / Global::ticks, timer, (char*)2);
		
		
		//Contains a while loop
		mainLoop();
		
		
		//Deletes all variables
		cleanup();
		
		
	} catch (std::runtime_error& e) {
		std::clog << "Fatal error : " << e.what() << std::endl;
		return;
	}
	
}

void Game::loadSettings() {
	std::map<std::string, int> settings;
	
	std::fstream file;
	
	file.open("settings.txt", std::ios::in);
	if (!file.good()) {
		generateDefaultSettings = true;
		return;
	}
	
	std::string key;
	int value;
	while (file >> key && file >> value) {
		settings[key] = value;
	}
	file.close();
	
	//TODO check degeneracies in width & height
	
	if (settings.find("width") != settings.end()) {
		Global::screenWidth = settings["width"];
	} else {
		throw SettingsLoadError();
	}
	
	if (settings.find("height") != settings.end()) {
		Global::screenHeight = settings["height"];
	} else {
		throw SettingsLoadError();
	}
	
	if (settings.find("fps") != settings.end()) {
		Global::fps = settings["fps"];
	} else {
		throw SettingsLoadError();
	}
}

void Game::init() {
	std::clog << "Initializing...";
	
	//SDL init
	std::clog << " SDL";
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw SDLInitError();
	}
	
	if (generateDefaultSettings) {
		//Gets the native resolution and saves it to the config file
		SDL_Rect displayRect;
		SDL_GetDisplayBounds(0, &displayRect);
		Global::screenWidth = displayRect.w;
		Global::screenHeight = displayRect.h;
		Global::fps = 60;
		std::fstream file;
		file.open("settings.txt", std::ios::out);
		if (!file.good()) {
			std::clog << "Error! Config file is not writeable" << std::endl;
		}
		
		file << "width" << "\t" << Global::screenWidth << std::endl;
		file << "height" << "\t" << Global::screenHeight << std::endl;
		file << "fps" << "\t" << Global::fps << std::endl;
		
		file.close();
	}
	
	//Window stuff
	Global::window = SDL_CreateWindow(Global::appName.c_str(),
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							Global::screenWidth,
							Global::screenHeight,
							0);
	if (Global::window == NULL) {
		throw SDLWindowError();
	}
	SDL_SetWindowFullscreen(Global::window, SDL_WINDOW_FULLSCREEN);
	
	//Cursor stuff
	SDL_ShowCursor(SDL_DISABLE);
	
	
	//Renderer init
	Global::renderer = SDL_CreateRenderer(Global::window, -1, SDL_RENDERER_ACCELERATED);
	if (Global::renderer == NULL) {
		throw SDLRendererError();
	}
	SDL_SetRenderDrawColor(Global::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//Setting blending mode
	SDL_SetRenderDrawBlendMode(Global::renderer, SDL_BLENDMODE_BLEND);
    
	
	//IMG module init
	std::clog << " SDL_IMG";
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
		throw SDLImageInitError();
	}
	
	
	//TTF module init
	std::clog << " TTF";
	if (TTF_Init() == -1) {
		throw TTFInitError();
	}
	
	
	//Mixer module opening
	std::clog << " SDL_MIXER";
	if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0) {
		throw SDLMixerInitError();
	}
	
	
	//Icon stuff
	SDL_Surface* tempIcon = IMG_Load("data/img/icon.png");
	SDL_SetWindowIcon(Global::window, tempIcon);
	SDL_FreeSurface(tempIcon);
	
	
	std::clog << " ...done" << std::endl;
}

uint32_t Game::timer(uint32_t ms, void* param) {
	SDL_Event ev;
	ev.type = SDL_USEREVENT;
	//Throws an annoying warning which we disable here
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
		ev.user.code = *((int*)(&param));
	#pragma GCC diagnostic pop
	SDL_PushEvent(&ev);
	return ms;
}

void Game::mainLoop() {
	bool quit = false;
	
	SDL_Event e;
	
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	
	while (SDL_WaitEvent(&e) && !quit) {
		switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				UserInputHandler::handleKeyPressEvent(e);
				break;
			case SDL_USEREVENT:
				if (e.user.code == 1) {
					//Runs every frame
					if (Global::player->getState() == PlayerState::MOVING) {
						Global::player->updatePlayerPosition();
						Global::npcHandler->updateNPCsPosition();
					}
					UserInputHandler::handleKeyDownEvent(keyboardState);
					Global::cursor->update();
					renderGame();
				} else if (e.user.code == 2) {
					Global::animationHandler->nextTick(Global::player->getState() == PlayerState::MOVING);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				UserInputHandler::handleMousePressEvent(e);
				break;
			case SDL_MOUSEWHEEL:
				//https://wiki.libsdl.org/SDL_MouseWheelEvent
				//Check if the mouse wheel is flipped
				if (e.wheel.direction == SDL_MOUSEWHEEL_NORMAL) {
					UserInputHandler::handleMouseWheelEvent(!(e.wheel.y-1));
				} else {
					UserInputHandler::handleMouseWheelEvent(e.wheel.y-1);
				}
				break;
		}
	}
}

void Game::renderGame() {
	SDL_RenderClear(Global::renderer);
	
	Global::map->render();
	Global::permaGUI->render();
	Global::guiHandler->render();
	Global::cursor->render();
	
	SDL_RenderPresent(Global::renderer);
}

void Game::cleanup() {
	std::clog << "Closing...";
	
	//Deletes global variables
	//Setting these pointers to NULL might be pointless but it would cause problems otherwise
	delete Global::guiHandler;
	Global::guiHandler = NULL;
	delete Global::permaGUI;
	Global::permaGUI = NULL;
	delete Global::cursor;
	Global::cursor = NULL;
	delete Global::npcHandler;
	Global::npcHandler = NULL;
	delete Global::worldObjectHandler;
	Global::worldObjectHandler = NULL;
	delete Global::map;
	Global::map = NULL;
	delete Global::unitHandler;
	Global::unitHandler = NULL;
	delete Global::itemHandler;
	Global::itemHandler = NULL;
	delete Global::resourceHandler;
	Global::resourceHandler = NULL;
	delete Global::animationHandler;
	Global::animationHandler = NULL;
	delete Global::camera;
	Global::camera = NULL;
	delete Global::player;
	Global::player = NULL;
	
	//Destroys core SDL objects
	SDL_DestroyRenderer(Global::renderer);
	SDL_DestroyWindow(Global::window);
	Global::window = NULL;
	Global::renderer = NULL;
	
	//Removing added SDL Timers
	SDL_RemoveTimer(frameID);
	SDL_RemoveTimer(tickID);
	
	//Quits SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	
	std::clog << " done" << std::endl;
	
	//Resetting clog
	log.close();
	std::clog.rdbuf(NULL);
}

void Game::quit() {
	SDL_Event ev;
	ev.type = SDL_QUIT;
	SDL_PushEvent(&ev);
}
