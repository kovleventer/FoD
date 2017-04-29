#pragma once

//Handles actual filesystem interactions (I hope so)
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include "exceptions.h"

class FilesystemHandler {
public:
	//Filesystem traversing methods
	static std::vector<std::string> getFilesInDir(std::string path);
	static std::vector<std::string> getDirsInDir(std::string path);
	static std::pair< std::vector<std::string>, std::vector<std::string> > getFilesAndDirsInDir(std::string path);
	
	//Extension handling methods
	static std::string removeExtension(std::string filename);
	static std::string getExtension(std::string filename);
};
