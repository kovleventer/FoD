#include "game.h"

#include "map.h"
#include "resourcehandler.h"
#include "itemhandler.h"
#include "unithandler.h"
#include "global.h"
#include "exceptions.h"
#include "userinputhandler.h"
#include "camera.h"

Game::Game(std::string aName, Version version) {
	//Passing metadata
	Global::appName = aName + version.toString();
	
	//So that means we have an (gameBoardWidth * tileSize) x (gameBoardHeight * tileSize) pixmap
	Global::gameBoardWidth = 32;
	Global::gameBoardHeight = 32;
	
	Global::defaultFontSize = 200;
	
	try {
		loadSettings();
		
		init();
		
		Global::camera = new Camera();
	
		Global::resourceHandler = new ResourceHandler();
		Global::resourceHandler->loadAll();
		
		Global::itemHandler = new ItemHandler();
		Global::itemHandler->loadAll();
		
		Global::unitHandler = new UnitHandler();
		
		Global::player = new Player(Point(5, 5));
		
		Global::permaGUI = new PermanentGUI();
		Global::guiHandler = new GUIHandler();
		
		//Generate map stuff
		//NOTE this must run after the media has been loaded
		Global::map = new Map(Global::gameBoardWidth, Global::gameBoardHeight);
		Global::map->createNPCPath();
		
		Global::cursor = new Cursor("impassable");
		
		//60 fps (or not)
		id = SDL_AddTimer(1000 / Global::fps, timer, NULL);
	} catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
		return;
	}
	
	//TODO better initialization system for player and npc inventory/army
	Global::player->getInventory()->addItem(Global::itemHandler->items["sword"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["shield"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["amulet"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["helmet"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["ring"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["staff"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["roundshield"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["poisonarrow"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["frostcloak"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["crossbow"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["sickle"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["boots"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["bow"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["ironarrow"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["wand"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["oldcloak"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["mshield1"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["mshield2"]);
	Global::player->getInventory()->addItem(Global::itemHandler->items["bread"]);
	
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Axeman", 1), UnitAddingPreference::FRONTROWFIRST);
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Axeman", 2), UnitAddingPreference::FRONTROWFIRST);
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Axeman", 3), UnitAddingPreference::FRONTROWFIRST);
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Axeman", 4), UnitAddingPreference::FRONTROWFIRST);
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Dog", 1), UnitAddingPreference::SUPPORTFIRST);
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Dog", 2), UnitAddingPreference::SUPPORTFIRST);
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Dog", 3), UnitAddingPreference::SUPPORTFIRST);
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Bowman", 1), UnitAddingPreference::BACKROWFIRST);
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Bowman", 2), UnitAddingPreference::BACKROWFIRST);
	Global::player->getArmy()->addUnit(Global::unitHandler->getUnit("Gray Mage", 1), UnitAddingPreference::BACKROWFIRST);
	
	
	//Contains a while loop
	mainLoop();
	
	cleanup();
}

void Game::loadSettings() {
	std::map<std::string, int> settings;
	
	std::fstream file;
	
	file.open("settings.txt", std::ios::in);
	std::string key;
	int value;
	while (file >> key && file >> value) {
		settings[key] = value;
	}
	
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
	
	if (settings.find("tile_size") != settings.end()) {
		Global::tileSize = settings["tile_size"];
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
	std::cout << "Initializing...";
	
	//SDL init
	std::cout << " SDL";
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw SDLInitError();
	}
	
	//Window stuff
	Global::window = SDL_CreateWindow(Global::appName.c_str(),
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							Global::screenWidth,
							Global::screenHeight,
							SDL_WINDOW_OPENGL);
	if (Global::window == NULL) {
		throw SDLWindowError();
	}
	SDL_SetWindowFullscreen(Global::window, SDL_WINDOW_FULLSCREEN);
	
	//Cursor stuff
	SDL_ShowCursor(SDL_DISABLE);
	
	
	//Renderer stuff
	Global::renderer = SDL_CreateRenderer(Global::window, -1, SDL_RENDERER_ACCELERATED);
	if (Global::renderer == NULL) {
		throw SDLRendererError();
	}
	SDL_SetRenderDrawColor(Global::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//Setting blending mode
	SDL_SetRenderDrawBlendMode(Global::renderer, SDL_BLENDMODE_BLEND);
    
	
	//IMG module stuff
	std::cout << " SDL_IMG";
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
		throw SDLImageInitError();
	}
	
	
	//TTF module stuff
	std::cout << " TTF";
	if (TTF_Init() == -1) {
		throw TTFInitError();
	}
    
    //TODO delete this
    SDL_SetWindowIcon(Global::window, IMG_Load("data/img/icon.png"));
	
	std::cout << " ...done" << std::endl;
}

Uint32 Game::timer(Uint32 ms, void* param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
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
				//handleKeyboardEvents(e);
				break;
			case SDL_USEREVENT:
				if (Global::player->getState() == PlayerState::MOVING) {
					Global::player->updatePlayerPosition();
					Global::map->updateNPCsPosition();
				}
				UserInputHandler::handleKeyDownEvent(keyboardState);
				Global::cursor->update();
				renderGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				UserInputHandler::handleMousePressEvent(e);
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
	std::cout << "Closing...";
	//Deletes global variables
	delete Global::guiHandler;
	delete Global::permaGUI;
	delete Global::cursor;
	delete Global::map;
	delete Global::unitHandler;
	delete Global::itemHandler;
	delete Global::resourceHandler;
	delete Global::camera;
	delete Global::player;
	
	//Destroys core SDL objects
	SDL_DestroyRenderer(Global::renderer);
	SDL_DestroyWindow(Global::window);
	Global::window = NULL;
	Global::renderer = NULL;
	
	//Quits SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	std::cout << " done" << std::endl;
}

void Game::quit() {
	SDL_Event ev;
	ev.type = SDL_QUIT;
	SDL_PushEvent(&ev);
}
