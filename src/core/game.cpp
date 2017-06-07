#include "game.h"

#include "../map/map.h"
#include "resourcehandler.h"
#include "itemhandler.h"
#include "unithandler.h"
#include "global.h"
#include "userinputhandler.h"
#include "../map/camera.h"
#include "npchandler.h"
#include "../util/filesystemhandler.h"
#include "worldobjecthandler.h"
#include "../map/player.h"
#include "../gui/popup.h"
#include "audiohandler.h"
#include "questhandler.h"

/*!
 * @author kovlev
 */

Game::Game(std::string aName, Version version) {
	generateDefaultSettings = false;
	
	//Passing metadata
	Global::appName = aName + version.toString();
	
	Global::defaultFontSize = 100;
	
	Global::tileSize = 64;
	
	//Changing this is not really safe
	Global::ticks = 64;
	
	try {
		//Creating log folder
		FilesystemHandler::createDirectory("log");
		
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
		
		std::clog << "Version: " << version.toString() << std::endl;
		
		try {
			Global::audioHandler = new AudioHandler(16);
			
			loadSettings();
			
			init();
			
			Global::camera = new Camera();
			
			Global::tickHandler = new TickHandler();
		
			Global::resourceHandler = new ResourceHandler();
			Global::resourceHandler->loadAll();
			
			Global::itemHandler = new ItemHandler();
			Global::itemHandler->loadAll();
			
			Global::unitHandler = new UnitHandler();
			
			Global::player = new Player("player", 0, 0);
			
			Global::permaGUI = new PermanentGUI();
			Global::permaGUI->initButtonCallbacks();
			
			Global::guiHandler = new GUIHandler();
			
			//Generate map stuff
			//NOTE this must run after the media has been loaded
			Global::map = new Map();
			Global::minimap->regenerateMinimap();
			
			Global::worldObjectHandler = new WorldObjectHandler();
			Global::worldObjectHandler->loadAll();
			
			//This must run after we load the world objects but before we load the npcs
			Global::map->createPassabilityMap();
			
			Global::npcHandler = new NPCHandler();
			Global::npcHandler->loadAll();
			
			Global::worldObjectHandler->setOwnershipRelations();
			
			Global::questHandler = new QuestHandler();
			Global::questHandler->loadAll();
			
			Global::cursor = new Cursor("impassable");
			
			
			//60 fps (or not)
			frameID = SDL_AddTimer(1000 / Global::fps, timer, (char*)1);
			tickID = SDL_AddTimer(1000 / Global::ticks, timer, (char*)2);
			
			
			//new Battle((NPC*)Global::npcHandler->getCharacterByName("Kov Lev"), (NPC*)Global::npcHandler->getCharacterByName("Friend"));
			
			
			//Contains a while loop
			mainLoop();
			
			
			//Deletes all variables
			cleanup();
			
			
		} catch (std::runtime_error& e) {
			//Catchng standard errors
			std::clog << "Fatal error : " << e.what() << std::endl;
		}
		
		//Resetting clog
		log.close();
		std::clog.rdbuf(NULL);
	} catch (...) {
		//Not using the redirected clog since it is not safe this case
		std::cout << "Logger fatal error or unhandled exception" << std::endl;
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
	//Reading settings form file
	while (file >> key && file >> value) {
		settings[key] = value;
	}
	file.close();
	
	if (settings.find("width") != settings.end()) {
		if (settings["width"] < MIN_WIDTH || settings["width"] > MAX_WIDTH) {
			std::clog << "Invalid width: " << settings["width"] << " Regenerating settings.txt." << std::endl;
			generateDefaultSettings = true;
		} else {
			Global::screenWidth = settings["width"];
		}
	} else {
		std::clog << "Error: Settings loading failed: no width value. Regenerating settings.txt." << std::endl;
		generateDefaultSettings = true;
	}
	
	if (settings.find("height") != settings.end()) {
		if (settings["height"] < MIN_HEIGHT || settings["height"] > MAX_HEIGHT) {
			std::clog << "Invalid height: " << settings["height"] << " Regenerating settings.txt." << std::endl;
			generateDefaultSettings = true;
		} else {
			Global::screenHeight = settings["height"];
		}
	} else {
		std::clog << "Error: Settings loading failed: no height value. Regenerating settings.txt." << std::endl;
		generateDefaultSettings = true;
	}
	
	if (settings.find("fps") != settings.end()) {
		if (settings["fps"] < MIN_FPS || settings["fps"] > MAX_FPS) {
			std::clog << "Invalid fps: " << settings["fps"] << " Regenerating settings.txt." << std::endl;
			generateDefaultSettings = true;
		} else {
			Global::fps = settings["fps"];
		}
	} else {
		std::clog << "Error: Settings loading failed: no fps value. Regenerating settings.txt." << std::endl;
		generateDefaultSettings = true;
	}
	
	if (settings.find("music_volume") != settings.end()) {
		if (settings["music_volume"] < 0 || settings["music_volume"] > MIX_MAX_VOLUME) {
			std::clog << "Invalid music volume: " << settings["music_volume"] << " Regenerating settings.txt." << std::endl;
			generateDefaultSettings = true;
		} else {
			Global::audioHandler->setMusicVolume(settings["music_volume"]);
		}
	} else {
		std::clog << "Error: Settings loading failed: no music_volume value. Regenerating settings.txt." << std::endl;
		generateDefaultSettings = true;
	}
	
	if (settings.find("sound_volume") != settings.end()) {
		if (settings["sound_volume"] < 0 || settings["sound_volume"] > MIX_MAX_VOLUME) {
			std::clog << "Invalid sound volume: " << settings["music_volume"] << " Regenerating settings.txt." << std::endl;
			generateDefaultSettings = true;
		} else {
			Global::audioHandler->setSoundVolume(settings["sound_volume"]);
		}
	} else {
		std::clog << "Error: Settings loading failed: no sound_volume value. Regenerating settings.txt." << std::endl;
		generateDefaultSettings = true;
	}
	
	
}

void Game::init() {
	std::clog << "Initializing...";
	
	//SDL init
	std::clog << " SDL";
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		throw std::runtime_error("SDL Init failed: " + std::string(SDL_GetError()));
	}
	
	if (generateDefaultSettings) {
		//Gets the native resolution and saves it to the config file
		SDL_Rect displayRect;
		SDL_GetDisplayBounds(0, &displayRect);
		Global::screenWidth = displayRect.w;
		Global::screenHeight = displayRect.h;
		Global::fps = 60;
		Global::audioHandler->setSoundVolume(MIX_MAX_VOLUME);
		Global::audioHandler->setMusicVolume(MIX_MAX_VOLUME);
		std::fstream file;
		file.open("settings.txt", std::ios::out);
		if (!file.good()) {
			std::clog << "Error! Config file is not writeable" << std::endl;
		}
		
		file << "width" << "\t" << Global::screenWidth << std::endl;
		file << "height" << "\t" << Global::screenHeight << std::endl;
		file << "fps" << "\t" << Global::fps << std::endl;
		file << "music_volume" << "\t" << Global::audioHandler->getMusicVolume() << std::endl;
		file << "sound_volume" << "\t" << Global::audioHandler->getSoundVolume() << std::endl;
		
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
		throw std::runtime_error("SDL window creation failed: " + std::string(SDL_GetError()));
	}
	SDL_SetWindowFullscreen(Global::window, SDL_WINDOW_FULLSCREEN);
	
	//Cursor stuff
	SDL_ShowCursor(SDL_DISABLE);
	
	
	//Renderer init
	Global::renderer = SDL_CreateRenderer(Global::window, -1, SDL_RENDERER_ACCELERATED);
	if (Global::renderer == NULL) {
		throw std::runtime_error("SDL renderer creation failed: " + std::string(SDL_GetError()));
	}
	SDL_SetRenderDrawColor(Global::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//Setting blending mode
	SDL_SetRenderDrawBlendMode(Global::renderer, SDL_BLENDMODE_BLEND);
    
	
	//IMG module init
	std::clog << " SDL_IMG";
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
		throw std::runtime_error("SDL_image init failed: " + std::string(IMG_GetError()));
	}
	
	
	//TTF module init
	std::clog << " TTF";
	if (TTF_Init() == -1) {
		throw std::runtime_error("SDL_ttf init failed: " + std::string(TTF_GetError()));
	}
	
	
	//Mixer module opening
	std::clog << " SDL_MIXER";
	int mixFlags = MIX_INIT_OGG;
	if((Mix_Init(mixFlags) & mixFlags) != mixFlags) {
		throw std::runtime_error("SDL_mixer init failed: " + std::string(Mix_GetError()));
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
					UserInputHandler::handleKeyDownEvent(keyboardState);
					Global::cursor->update();
					renderGame();
				} else if (e.user.code == 2) {
					Global::tickHandler->nextTick(Global::player->getState() == PlayerState::MOVING || Global::player->getState() == PlayerState::WAITING);
					if (Global::player->getState() == PlayerState::MOVING) {
						Global::player->updatePlayerPosition();
						Global::npcHandler->updateNPCsPosition();
					}
					if (Global::player->getState() == PlayerState::WAITING) {
						Global::npcHandler->updateNPCsPosition();
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				UserInputHandler::handleMousePressEvent(e);
				break;
			case SDL_MOUSEBUTTONUP:
				UserInputHandler::handleMouseReleaseEvent(e);
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
	
	Battle::isMainLoopEnded = true;
	
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
	delete Global::audioHandler;
	Global::audioHandler = NULL;
	delete Global::resourceHandler;
	Global::resourceHandler = NULL;
	delete Global::tickHandler;
	Global::tickHandler = NULL;
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
}

void Game::quit() {
	SDL_Event ev;
	ev.type = SDL_QUIT;
	SDL_PushEvent(&ev);
}

void Game::checkPerformance() {
	clock_t begin, end;
	std::clog << "Performance test started (" << TESTCOUNT << " cycles)" << std::endl;
	
	begin = clock();
	for (int i = 0; i < TESTCOUNT; i++) {
		Global::map->render();
	}
	end = clock();
	std::clog << "\tMap rendering took " << double(end - begin) / CLOCKS_PER_SEC << " seconds" << std::endl;
	
	begin = clock();
	for (int i = 0; i < TESTCOUNT; i++) {
		Global::permaGUI->render();
	}
	end = clock();
	std::clog << "\tHUD rendering took " << double(end - begin) / CLOCKS_PER_SEC << " seconds" << std::endl;
	
	begin = clock();
	for (int i = 0; i < TESTCOUNT; i++) {
		Global::guiHandler->render();
	}
	end = clock();
	std::clog << "\tGUI rendering took " << double(end - begin) / CLOCKS_PER_SEC << " seconds" << std::endl;
	
	begin = clock();
	for (int i = 0; i < TESTCOUNT; i++) {
		Global::cursor->render();
	}
	end = clock();
	std::clog << "\tCursor rendering took " << double(end - begin) / CLOCKS_PER_SEC << " seconds" << std::endl;
}

const int Game::MIN_WIDTH = 200;
const int Game::MAX_WIDTH = 8000;
const int Game::MIN_HEIGHT = 200;
const int Game::MAX_HEIGHT = 8000;
const int Game::MIN_FPS = 10;
const int Game::MAX_FPS = 500;
const int Game::TESTCOUNT = 10000;

