#!/bin/bash

EXECUTABLE="FallOfDyrangor"

SOURCES=(
	"game.cpp"
	"main.cpp"
	"point.cpp"
	"tile.cpp"
	"map.cpp"
	"resourcehandler.cpp"
	"worldobject.cpp"
	"global.cpp"
	"exceptions.cpp"
	"cursor.cpp"
	"userinputhandler.cpp"
	"camera.cpp"
	"player.cpp"
	"pathfinding.cpp"
	"interactiveworldobject.cpp"
	"npc.cpp"
	"mapentity.cpp"
	"circularpath.cpp"
	"permanentgui.cpp"
	"button.cpp"
	"guihandler.cpp"
	"guipart.cpp"
	"transientgui.cpp"
	"wholescreengui.cpp"
	"popup.cpp"
	"text.cpp"
	"inventory.cpp"
	"item.cpp"
	"itemhandler.cpp"
	"iteminfo.cpp"
	"unit.cpp"
	"army.cpp"
	"unithandler.cpp"
	"unitinfo.cpp"
	"abstractunit.cpp"
	"battle.cpp"
	"version.cpp"
	"unitinventoryhandler.cpp"
)

LINKER=(
	"-lSDL2"
	"-lSDL2_image"
	"-lSDL2_ttf"
	"-lSDL2_mixer"
)
 
if [[ $1 == "R" || $1 == "r" ]]; then
	if [ -a $EXECUTABLE ]; then
		echo "Running executable..."
		./$EXECUTABLE
	else
		echo "Executable not found"
	fi
elif [[ $1 == "F" || $1 == "f" ]]; then
	ts=$(date +%s%N)
	echo "Fully Compiling..."
	mkdir -p o
	for i in ${SOURCES[@]}; do
		g++ -O3 -Wall -std=c++0x -c "src/"$i -o "o/${i%%.*}.o"
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
			g++ -O3 -Wall -std=c++0x -c "src/"$i -o "o/${i%%.*}.o"
			echo " $i"
			MODIFIED=true
		fi
		
		if [[ "src/"$i -nt "o/${i%%.*}.o" ]]; then
			g++ -O3 -Wall -std=c++0x -c "src/"$i -o "o/${i%%.*}.o"
			echo " $i"
			MODIFIED=true
		else
			if [ -f "src/${i%%.*}.h" ]; then
				if [[ "src/${i%%.*}.h" -nt "o/${i%%.*}.o" ]]; then
					g++ -O3 -Wall -std=c++0x -c "src/"$i -o "o/${i%%.*}.o"
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
	rm -rf o
	rm $EXECUTABLE
	echo "Clearing done"
else
	echo "Invalid options, use 'c', 'r', 'f' or 'm'"
fi
