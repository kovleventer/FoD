#pragma once

#include <string>
#include <SDL_mixer.h>

/*!
 * @author kovlev
 */

/*!
 * @class AudioHandler Handles audio
 * Plays music and sound effects
 * Basically a wrapper over SDL_Mixer
 */
class AudioHandler {
public:
	AudioHandler(int channels);
	~AudioHandler();
	
	//Plays a chunk on some channel (-1 is given)
	void playSoundEffect(std::string name);
	
	//Plays a given music
	void playMusic(std::string name);
	
	//Whether music is playing
	bool isPlayingMusic();
	
	//Volume handling functions
	void muteAll();
	void muteSounds();
	void muteMusic();
	
	void unmuteAll();
	void unmuteSounds();
	void unmuteMusic();
	
	bool isSoundMuted();
	bool isMusicMuted();
	
	void setSoundVolume(int newVolume);
	void setMusicVolume(int newVolume);
	
	//Interpolation is done at methods with argument as double
	//0 means muted and 1 means max volume
	void setSoundVolume(double newVolume);
	void setMusicVolume(double newVolume);
	
	int getSoundVolume();
	int getMusicVolume();
private:
	//Not really used since the param -1 handles all channels
	//Therefore iterating over channels is not have to be done by us
	int allocatedChannels;
	
	//Temporary variables for muting
	int soundVolume, musicVolume;
};
