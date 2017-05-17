# Fall of Dyrangor
![Logo][logo] A small role-playing/strategy game that I developed using the SDL2 library on C++. ![Logo][logo]

[logo]: data/img/icon.png?raw=true

# Libraries used
* C++ standard library (using the c++14 standard)
* SDL2
* SDL2_image for texture loading
* SDL2_mixer for handling sound
* SDL2_ttf for font rendering

# Compilation
## GNU/Linux
To obtain SDL development libraries under Ubuntu, type
```
sudo apt-get install libsdl2*
```
In the case of different distros that are using different package managers, you are going to have to use their own commands (such as yum, dnf etc.). For compilation, just run the make script (which is not an actual makefile, just a shell script) with an 'm' or 'f' parameter. Uses g++.

## Windows

### Command line using gcc
1. Download and install a gcc compiler. Using [this one](http://tdm-gcc.tdragon.net/) is recommended due to pthread support (although not necessary). You can get either the 32 or the 64 bit version. Since the included buildscript calls g++ directly, it is recommended to set PATH correctly (or use a compiler which does this automatically). Providing arguments for *winmake.bat*, one can specify the target architecture. If no argument is given, the script tries to do detect the architecture by itself. This is not recommended though, as it is very inconsistent.
2. Download the SDL development libraries ([SDL][SDL], [SDL_image][SDL_image], [SDL_mixer][SDL_mixer], [SDL_ttf][SDL_ttf]). Make sure to select the ones made for MinGW.
3. Extract the downloaded files into the folder the compiler is in. The source folder depends on the architecture, while the destination is always C:/TDM-GCC/mingw32 (there might be a prefix before mingw). Either way, the folder structures (include, lib, and possibly share or bin) should match.
4. In order to compile successfully, you will need to add the SDL include and lib files to your compiler.
5. Clone this project.
6. To start the compilation process, run *winmake.bat* with an argument ('32' or '64', depending on which architecture you want to compile it for). In order to run it with said arguments, you will most likely require a cmd window that is pointing here (which you can easily achieve by shift-right clicking in the explorer window, and then choosing "Open Command Line here").
7. If everything succeeds, a .exe file will be generated in build/. In order to run it, you will have to do the followings:
	* Copy the required .dlls (*SDL2.dll*, *SDL2_image.dll*, *SDL2_mixer.dll*, *SDL2_ttf.dll* as well as the other needed ones, such as zlib and libpng) to the same directory as the executable is in. If you accidentally mixed up x86 and x64 .dlls, you can use [this script](test/windows_arch_checker.py) which helps with integrity checking.
	* Move the *data* folder to the directory the executable is in.
8. You should be able to run the game now.

### Visual studio 2017 using Microsoft's C++ compiler
1. IMPORTANT: The standard SDL2 development library does not work with Visual Studio versions newer than 2013. To resolve this, you are going to require this one: [SDL-visualstudio](https://buildbot.libsdl.org/sdl-builds/sdl-visualstudio/) (select the one on the top).
2. Download and extract the remaining SDL development libraries ([SDL_image][SDL_image], [SDL_mixer][SDL_mixer], [SDL_ttf][SDL_ttf]). Make sure to select the ones made for Visual C++. 
3. Clone this repository.
4. Create a new empty project in Visual Studio.
5. Drag 'n Drop the src files from the cloned project to the *Source Files* folder in the Solution Explorer.
6. Right click the project in the Solution Explorer and select *Properties* as shown.
![vs1](http://i.imgur.com/od5LVYi.png)
7. Edit the include directories so that they point to the include directories in your extracted SDL folders as shown.
![vs2](http://i.imgur.com/vzZ2itf.png)
8. Do the same with the library directories, this time pointing to the lib\x86 or lib\win32 folders.
![vs3](http://i.imgur.com/tO0tC51.png)
9. Go to Linker->Input, edit Additional Dependencies and paste the following in the first input field: *SDL2.lib;SDL2main.lib;SDL2_image.lib;SDL2_ttf.lib;SDL2_mixer.lib*
![vs4](http://i.imgur.com/lAJV9z0.png)
10. If you've done everything correctly, you should be able to build the project now.
![vs5](http://i.imgur.com/sc0nqWt.png)
11. In order to run the project, copy the required .dlls (*SDL2.dll*, *SDL2_image.dll*, *SDL2_mixer.dll*, *SDL2_ttf.dll* etc.) as well as the *log* and the *data* folders into the same directory as the executable is in.
12. You should be able to run the game now.

## macOS
Not tested yet

[SDL]: https://www.libsdl.org/download-2.0.php
[SDL_image]: https://www.libsdl.org/projects/SDL_image/
[SDL_mixer]: https://www.libsdl.org/projects/SDL_mixer/
[SDL_ttf]: https://www.libsdl.org/projects/SDL_ttf/

# Notes and specs
This section will be added later.

[//]: # (authors: kovlev, Bobobot)
