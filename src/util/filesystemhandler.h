#pragma once

#if (defined(WIN32) || defined(_WIN32) ||defined (__WIN32__))
//Using windows header on windows (techincally c++17 could be used, but won't be yet TODO later)
#include <windows.h>
#else
//On unix systems we can use the dirent.h for filesystem traversion and sys ones for folder creation
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include "exceptions.h"

/*!
 * @author kovlev
 */


/*!
 * @class FilesystemHandler contains helper functions on listing all files and directories in a folder
 * Also can be used for dealing with extensions
 */
class FilesystemHandler {
public:
	//Filesystem traversing methods
	static std::vector<std::string> getFilesInDir(std::string path);
	static std::vector<std::string> getDirsInDir(std::string path);
	static std::pair< std::vector<std::string>, std::vector<std::string> > getFilesAndDirsInDir(std::string path);
	
	//Extension handling methods
	static std::string removeExtension(std::string filename);
	static std::string getExtension(std::string filename);
	
	//Filesystem modifying methods
	static void createDirectory(std::string path);
private:
#if (defined(WIN32) || defined(_WIN32) ||defined (__WIN32__))
	//Helper function related to windows path
	static std::string toWindowsPath(std::string path);
#endif
};
