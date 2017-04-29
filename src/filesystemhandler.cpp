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
				//If directory we skip that
				if (entry->d_type == DT_DIR) {
					std::clog << "Warning: Skipping directory in " << path << ": " << entry->d_name << std::endl;
					continue;
				}
				
				fileList.push_back(filename);
			}
		}
		closedir(directory);
	} else {
		 throw FilesystemError();
	}
	return fileList;
}

std::vector<std::string> FilesystemHandler::getDirsInDir(std::string path) {
	//The list which we are going to return
	std::vector<std::string> dirList;
	
	//dirent.h stuff
	//opendir requires a c-type string
	DIR* directory = opendir(path.c_str());
	struct dirent* entry;
	
	if (directory != NULL) {
		//While we have a remaining entry
		while ((entry = readdir(directory)) != NULL) {
			std::string dirname = entry->d_name;
			
			//Filenames starting with a dot are not valid so we skip them
			if (dirname.find_first_of(".") != 0) {
				//If not directory we skip that
				if (entry->d_type != DT_DIR) {
					std::clog << "Warning: Skipping file in " << path << ": " << entry->d_name << std::endl;
					continue;
				}
				
				dirList.push_back(dirname);
			}
		}
		closedir(directory);
	} else {
		 throw FilesystemError();
	}
	return dirList;
}

std::pair< std::vector<std::string>, std::vector<std::string> > FilesystemHandler::getFilesAndDirsInDir(std::string path) {
	std::vector<std::string> fileList;
	std::vector<std::string> directoryList;
	
	//dirent.h stuff
	//opendir requires a c-type string
	DIR* directory = opendir(path.c_str());
	struct dirent* entry;
	
	if (directory != NULL) {
		//While we have a remaining entry
		while ((entry = readdir(directory)) != NULL) {
			std::string entryName = entry->d_name;
			
			//Filenames starting with a dot are not valid so we skip them
			if (entryName.find_first_of(".") != 0) {
				if (entry->d_type == DT_DIR) {
					directoryList.push_back(entryName);
				} else {
					fileList.push_back(entryName);
				}
			}
		}
		closedir(directory);
	} else {
		 throw FilesystemError();
	}
	//The first enty is the filelist, the second entry is the dirlist
	return std::pair< std::vector<std::string>, std::vector<std::string> >(fileList, directoryList);
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

std::string FilesystemHandler::getExtension(std::string filename) {
	if (filename.find_last_of(".") == std::string::npos) return "";
	return filename.substr(filename.find_last_of(".") + 1);
}
