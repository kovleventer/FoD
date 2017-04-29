#pragma once

//Handles actual filesystem interactions (I hope so)
#include <dirent.h>
//TODO check this on windows
#include <string>
#include <vector>

#include "exceptions.h"

class FilesystemHandler {
public:
	static std::vector<std::string> getFilesInDir(std::string path);
	
	static std::string removeExtension(std::string filename);
};
