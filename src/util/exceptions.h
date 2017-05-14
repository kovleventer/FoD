#pragma once

#include <stdexcept>
#include <string>

/*!
 * @author kovlev
 * All classes are inherited from exceptions
 * Their names are self-explanatory hopefully
 * TODO remove this all; makes no sense
 */

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

class MediaNotFoundError : public std::runtime_error {
public:
	MediaNotFoundError(std::string name);
	const char* what() const throw();
private:
	std::string errorMessage;
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

class FilesystemError : public std::runtime_error {
public:
	FilesystemError();
	const char* what() const throw();
private:
	static std::string errorMessage;
};
