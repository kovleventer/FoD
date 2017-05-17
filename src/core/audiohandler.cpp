#include "audiohandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

AudioHandler::AudioHandler(int channels) {
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		throw std::runtime_error("Mixer open audio failed: " + std::string(Mix_GetError()));
	}
	allocatedChannels = Mix_AllocateChannels(channels);
}

AudioHandler::~AudioHandler() {
	Mix_CloseAudio();
}

void AudioHandler::playSoundEffect(std::string name) {
	Mix_PlayChannel(-1, Global::resourceHandler->getSound(name), 0);
}

void AudioHandler::playMusic(std::string name) {
	Mix_PlayMusic(Global::resourceHandler->getMusic(name), 0);
}

bool AudioHandler::isPlayingMusic() {
	return Mix_PlayingMusic();
}

void AudioHandler::muteAll() {
	if (!isSoundMuted()) {
		muteSounds();
	}
	if (!isMusicMuted()) {
		muteMusic();
	}
}

void AudioHandler::muteSounds() {
	soundVolume = Mix_Volume(-1, -1);
	Mix_Volume(-1, 0);
}

void AudioHandler::muteMusic() {
	musicVolume = Mix_VolumeMusic(0);
}

void AudioHandler::unmuteAll() {
	if (isSoundMuted()) {
		unmuteSounds();
	}
	if (isMusicMuted()) {
		unmuteMusic();
	}
}

void AudioHandler::unmuteSounds() {
	Mix_Volume(-1, soundVolume);
}

void AudioHandler::unmuteMusic() {
	Mix_VolumeMusic(musicVolume);
}

bool AudioHandler::isSoundMuted() {
	return Mix_Volume(-1, -1) == 0;
}

bool AudioHandler::isMusicMuted() {
	return Mix_VolumeMusic(-1) == 0;
}

void AudioHandler::setSoundVolume(int newVolume) {
	Mix_Volume(-1, newVolume);
}

void AudioHandler::setMusicVolume(int newVolume) {
	Mix_VolumeMusic(newVolume);
}

void AudioHandler::setSoundVolume(double newVolume) {
	setSoundVolume(newVolume * MIX_MAX_VOLUME);
}

void AudioHandler::setMusicVolume(double newVolume) {
	setMusicVolume(newVolume * MIX_MAX_VOLUME);
}

int AudioHandler::getSoundVolume() {
	return Mix_Volume(-1, -1);
}

int AudioHandler::getMusicVolume() {
	return Mix_VolumeMusic(-1);
}
