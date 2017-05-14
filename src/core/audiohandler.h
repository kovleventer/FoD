#pragma once

#include <string>
#include <SDL_mixer.h>

/*!
 * @author kovlev
 */

/*!
 * @class AudioHandler Handles audio
 * Plays music and sound effects
 */
class AudioHandler {
public:
	AudioHandler();
	
	//Plays a chunk on some channel (-1 is given)
	void playSoundEffect(std::string name);
	
	//Plays a given music
	void playMusic(std::string name);
	
	bool isPlayingMusic();
};
