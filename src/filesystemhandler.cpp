#include "filesystemhandler.h"

std::vector<std::string> FilesystemHandler::getFilesInDir(std::string path) {
	//The list which we are going to return
	std::vector<std::string> fileList;
	
	//dirent.h stuff
	//opendir requires a c-type string
	DIR* directory = opendir(path.c_str());
	struct dirent* entry;
	
	if (directory != NULL) {
		//While we have a remaining entry
		while ((entry = readdir(directory)) != NULL) {
			std::string filename = entry->d_name;
			
			//Filenames starting with a dot are not valid so we skip them
			if (filename.find_first_of(".") != 0) {
				fileList.push_back(filename);
			}
		}
		closedir(directory);
	} else {
		 throw FilesystemError();
	}
	return fileList;
}

std::string FilesystemHandler::removeExtension(std::string filename) {
	//Removes file extension
	//https://stackoverflow.com/questions/6417817/easy-way-to-remove-extension-from-a-filename
	//NOTE does not handles some extensions well (tar.gz for example)
	size_t lastdot = filename.find_last_of(".");
	if (lastdot != std::string::npos) {
		//If we have a dot, we cut of the part that comes after this dot
		filename = filename.substr(0, lastdot);
	}
	return filename;
}
