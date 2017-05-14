#!/bin/bash

EXECUTABLE="FallOfDyrangor"
EXECUTABLE_DEBUG="FoD_DEBUG"

SOURCES=(
	"main.cpp"
	
	"core/game.cpp"
	"core/resourcehandler.cpp"
	"core/global.cpp"
	"core/cursor.cpp"
	"core/userinputhandler.cpp"
	"core/battle.cpp"
	"core/npchandler.cpp"
	"core/worldobjecthandler.cpp"
	"core/animationhandler.cpp"
	"core/animatabletexture.cpp"
	"core/unithandler.cpp"
	"core/itemhandler.cpp"
	"core/audiohandler.cpp"
	
	"gui/permanentgui.cpp"
	"gui/button.cpp"
	"gui/guihandler.cpp"
	"gui/wholescreengui.cpp"
	"gui/popup.cpp"
	"gui/interactivegui.cpp"
	"gui/basicgui.cpp"
	
	"map/tile.cpp"
	"map/map.cpp"
	"map/worldobject.cpp"
	"map/camera.cpp"
	"map/player.cpp"
	"map/npc.cpp"
	"map/mapentity.cpp"
	"map/minimap.cpp"
	"map/character.cpp"
	
	"player/inventory.cpp"
	"player/item.cpp"
	"player/iteminfo.cpp"
	"player/unit.cpp"
	"player/army.cpp"
	"player/unitinfo.cpp"
	"player/abstractunit.cpp"
	"player/unitinventoryhandler.cpp"
	
	"util/exceptions.cpp"
	"util/pathfinding.cpp"
	"util/circularpath.cpp"
	"util/point.cpp"
	"util/text.cpp"
	"util/version.cpp"
	"util/filesystemhandler.cpp"
)

LINKER=(
	"-lSDL2"
	"-lSDL2_image"
	"-lSDL2_ttf"
	"-lSDL2_mixer"
	"-lpthread"
)

FLAGS="-O3 -Wall `sdl2-config --cflags` -std=c++14 -c"
DEBUGFLAGS="-O0 -g -Wall `sdl2-config --cflags` -std=c++14 -c"

if [[ $1 == "R" || $1 == "r" ]]; then
	if [ -a $EXECUTABLE ]; then
		echo "Running executable..."
		./$EXECUTABLE
	else
		echo "Executable not found"
	fi
elif [[ $1 == "D" || $1 == "d" ]]; then
	ts=$(date +%s%N)
	echo "Fully Compiling Debug Version..."
	mkdir -p o
	for i in ${SOURCES[@]}; do
		mkdir -p "o/$(dirname "${i}")"
		g++ $DEBUGFLAGS "src/"$i -o "o/${i%%.*}d.o"
		echo " $i"
	done
	
	LINKCOMMAND="g++ -o $EXECUTABLE_DEBUG"
	for i in ${SOURCES[@]}; do
		LINKCOMMAND=$LINKCOMMAND" o/${i%%.*}d.o"
	done
	echo "Linking"
	for i in ${LINKER[@]}; do
		LINKCOMMAND=$LINKCOMMAND" $i"
	done
	eval $LINKCOMMAND
	echo "Compilation finished"
	tt=$((($(date +%s%N) - $ts)/1000000))
	echo "Time taken: $tt ms"
elif [[ $1 == "F" || $1 == "f" ]]; then
	ts=$(date +%s%N)
	echo "Fully Compiling..."
	mkdir -p o
	for i in ${SOURCES[@]}; do
		mkdir -p "o/$(dirname "${i}")"
		g++ $FLAGS "src/"$i -o "o/${i%%.*}.o"
		echo " $i"
	done
	
	LINKCOMMAND="g++ -o $EXECUTABLE "
	for i in ${SOURCES[@]}; do
		LINKCOMMAND=$LINKCOMMAND" o/${i%%.*}.o"
	done
	echo "Linking"
	for i in ${LINKER[@]}; do
		LINKCOMMAND=$LINKCOMMAND" $i"
	done
	eval $LINKCOMMAND
	echo "Compilation finished"
	tt=$((($(date +%s%N) - $ts)/1000000))
	echo "Time taken: $tt ms"
elif [[ $1 == "M" || $1 == "m" ]]; then
	ts=$(date +%s%N)
	echo "Compiling..."
	if [ ! -d "o" ]; then
		mkdir -p o
	fi
	
	MODIFIED=false
	for i in ${SOURCES[@]}; do
		if [ ! -f "o/${i%%.*}.o" ]; then
			mkdir -p "o/$(dirname "${i}")"
			g++ $FLAGS "src/"$i -o "o/${i%%.*}.o"
			echo " $i"
			MODIFIED=true
		fi
		
		if [[ "src/"$i -nt "o/${i%%.*}.o" ]]; then
			mkdir -p "o/$(dirname "${i}")"
			g++ $FLAGS "src/"$i -o "o/${i%%.*}.o"
			echo " $i"
			MODIFIED=true
		else
			if [ -f "src/${i%%.*}.h" ]; then
				if [[ "src/${i%%.*}.h" -nt "o/${i%%.*}.o" ]]; then
					mkdir -p "o/$(dirname "${i}")"
					g++ $FLAGS "src/"$i -o "o/${i%%.*}.o"
					echo " $i"
					MODIFIED=true
				fi
			fi
		fi
	done
	
	if [ "$MODIFIED" = true ]; then
		LINKCOMMAND="g++ -o $EXECUTABLE "
		for i in ${SOURCES[@]}; do
			LINKCOMMAND=$LINKCOMMAND" o/${i%%.*}.o"
		done
		echo "Linking"
		for i in ${LINKER[@]}; do
			LINKCOMMAND=$LINKCOMMAND" $i"
		done
		eval $LINKCOMMAND
	else
		echo "Nothing to update"
	fi
	
	echo "Compilation finished"
	tt=$((($(date +%s%N) - $ts)/1000000))
	echo "Time taken: $tt ms"
elif [[ $1 == "C" || $1 == "c" ]]; then
	echo "Clearing..."
	#TODO clearing should handle build folder
	rm -rf o
	rm $EXECUTABLE
	rm $EXECUTABLE_DEBUG
	echo "Clearing done"
else
	echo "Invalid options, use 'c', 'r', 'f', 'd' or 'm'"
fi
