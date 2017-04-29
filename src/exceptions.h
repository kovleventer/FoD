#pragma once

#include <stdexcept>
#include <string>

//My exceptions for further use
class SDLInitError : public std::runtime_error {
public:
	SDLInitError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};

class SDLWindowError : public std::runtime_error {
public:
	SDLWindowError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};

class SDLRendererError : public std::runtime_error {
public:
	SDLRendererError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};

class SDLImageInitError : public std::runtime_error {
public:
	SDLImageInitError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};

class TTFInitError : public std::runtime_error {
public:
	TTFInitError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};

class SDLMixerInitError : public std::runtime_error {
public:
	SDLMixerInitError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};

//TODO error messages (which media file is missing)
class MediaNotFoundError : public std::runtime_error {
public:
	MediaNotFoundError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};

class TextureConvertError : public std::runtime_error {
public:
	TextureConvertError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};

class MapLoadError : public std::runtime_error {
public:
	MapLoadError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};

class SettingsLoadError : public std::runtime_error {
public:
	SettingsLoadError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};
