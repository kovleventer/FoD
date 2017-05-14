#include "filesystemhandler.h"

/*!
 * @author kovlev
 */
//TODO check for unicode support and possibly add it

std::vector<std::string> FilesystemHandler::getFilesInDir(std::string path) {
	//The list which we are going to return
	std::vector<std::string> fileList;
	
#if (defined(WIN32) || defined(_WIN32) ||defined (__WIN32__))
	//Windows implementation
	
	HANDLE hFind;
	WIN32_FIND_DATA data;
	
	hFind = FindFirstFile(toWindowsPath(path).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.cFileName[0] != '.') {
				std::string entryName = data.cFileName;
				if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					//Does not check for specific files. It assumes everything is a file which is not a directory
					fileList.push_back(entryName);
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	
#else
	
	//Unix implementation with dirent.h
	
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
	
#endif

	return fileList;
}

std::vector<std::string> FilesystemHandler::getDirsInDir(std::string path) {
	//The list which we are going to return
	std::vector<std::string> dirList;
	
#if (defined(WIN32) || defined(_WIN32) ||defined (__WIN32__))
	//Windows implementation
	
	HANDLE hFind;
	WIN32_FIND_DATA data;
	
	hFind = FindFirstFile(toWindowsPath(path).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.cFileName[0] != '.') {
				std::string entryName = data.cFileName;
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					dirList.push_back(entryName);
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	
#else
	
	//Unix implementation
	
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
	
#endif

	return dirList;
}

std::pair< std::vector<std::string>, std::vector<std::string> > FilesystemHandler::getFilesAndDirsInDir(std::string path) {
	//The lists to return
	std::vector<std::string> fileList;
	std::vector<std::string> directoryList;
	
#if (defined(WIN32) || defined(_WIN32) ||defined (__WIN32__))
	//Windows implementation
	
	HANDLE hFind;
	WIN32_FIND_DATA data;
	
	hFind = FindFirstFile(toWindowsPath(path).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.cFileName[0] != '.') {
				std::string entryName = std::string(data.cFileName);
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					directoryList.push_back(entryName);
				} else {
					fileList.push_back(entryName);
				}
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	
#else
	//Unix implementation
	
	
	
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
	
#endif

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

void FilesystemHandler::createDirectory(std::string path) {
#if (defined(WIN32) || defined(_WIN32) ||defined (__WIN32__))
	//Windows implementation
	if (!(CreateDirectory(path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())) {
		throw std::runtime_error("File creation unsuccessful");
	}
#else
	//Unix implementation
	//NOTE no error checking here
	mkdir(path.c_str(), 0777);
#endif
}

#if (defined(WIN32) || defined(_WIN32) ||defined (__WIN32__))
std::string FilesystemHandler::toWindowsPath(std::string path) {
	//Fairly easy and clean method hopedully
	//Yet still led me to 5 hours of debugging
	if (path.back() == '/') {
		return path.append("*");
	} else {
		return path.append("/*");
	}
}
#endif
