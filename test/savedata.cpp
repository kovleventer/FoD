#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <string>
#include <sstream>

#include <zlib.h>

#include "../src/util/filesystemhandler.h"

/*!
 * @author kovlev
 * 
 * A test file to compress the data folder into one file
 */

std::vector< std::pair <std::string, int> > paths;

void parseFolder(std::string folderName) {
	std::pair< std::vector<std::string>, std::vector<std::string> > entries = FilesystemHandler::getFilesAndDirsInDir(folderName);
	for (unsigned int i = 0; i < entries.first.size(); i++) {
		paths.push_back({folderName + entries.first[i], FilesystemHandler::getFileSize(entries.first[i])});
	}
	for (unsigned int i = 0; i < entries.second.size(); i++) {
		//Recursive call
		parseFolder(folderName + entries.second[i] + "/");
	}
}

int main() {
	int mode;
	std::cin >> mode;
	
	if (mode == 0) {
		parseFolder("../data/");
		std::fstream out;
		out.open("data.sav", std::ios::out | std::ios::binary);
		
		out << paths.size() << std::endl;
		for (unsigned int i = 0; i < paths.size(); i++) {
			out << paths[i].first << "\t" << paths[i].second << std::endl;
		}
		
		for (unsigned int i = 0; i < paths.size(); i++) {
			
			char c;
			std::fstream in;
			in.open(paths[i].first, std::ios::in | std::ios::binary);
			std::cout << paths[i].first << std::endl;
			while (in.read(&c, 1)) {
				out << c;
			}
			
			in.close();
		}
		
		
		out.close();
	} else {
		std::fstream in;
		in.open("data.sav", std::ios::in | std::ios::binary);
		
		unsigned int fileCount;
		std::string temp;
		std::getline(in, temp);
		fileCount = std::stoi(temp);
		for (unsigned int i = 0; i < fileCount; i++) {
			std::string l;
			std::getline(in, l);
			std::istringstream iss(l);
			std::string path;
			int size;
			std::getline(iss, path, '\t');
			iss >> size;
			paths.push_back({path, size});
		}
		
		for (unsigned int i = 0; i < paths.size(); i++) {
			std::istringstream iss(paths[i].first);
			std::ostringstream oss;
			
			std::string substr;
			std::getline(iss, substr, '/');
			while (iss.good()){
				oss << substr;
				if (substr != "..") {
					FilesystemHandler::createDirectory(oss.str());
				}
				oss << '/';
				std::getline(iss, substr, '/');
			}
			
			std::fstream out;
			out.open(paths[i].first, std::ios::out | std::ios::binary);
			std::cout << paths[i].first << std::endl;
			for (unsigned int j = 0; j < paths[i].second; j++) {
				char c;
				in.read(&c, 1);
				out << c;
			}
			out.close();
		}
		
		in.close();
	}
}
