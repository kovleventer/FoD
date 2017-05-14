#include "audiohandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

AudioHandler::AudioHandler() {}

void AudioHandler::playSoundEffect(std::string name) {
	Mix_PlayChannel(-1, Global::resourceHandler->getSound(name), 0);
}

void AudioHandler::playMusic(std::string name) {
	Mix_PlayMusic(Global::resourceHandler->getMusic(name), 0);
}

bool AudioHandler::isPlayingMusic() {
	return Mix_PlayingMusic();
}
