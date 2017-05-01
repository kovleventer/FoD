@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

if not exist "build/32" mkdir "build/32"
if not exist "build/64" mkdir "build/64"

set flags=-O3 -Wall -std=c++14
set linker=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -mwindows

For /f %%a in ('dir /b /s "src\*.cpp"') do (
	set start=%%~da
	set folder=%%~pa
	set file=%%~na.o
	echo Compiling !start!!folder!!file!
	set folder=!folder:src=o_win!
	if not exist !start!!folder! mkdir !start!!folder!
	set obj=!obj!!start!!folder!!file! 
	if /I "%1" == "32" (
		g++ -m32 !flags! -o !start!!folder!!file! -c %%a
	) else (
		if /I "%1" == "64" (
			g++ -m64 !flags! -o !start!!folder!!file! -c %%a
		) else (
			g++ !flags! -o !start!!folder!!file! -c %%a
		)
	)
)

echo Linking...
if /I "%1" == "32" (
	g++ -m32 -o build/32/FallOfDyrangor_WINDOWS.exe !obj! !linker!
) else (
	if /I "%1" == "64" (
		g++ -m64 -o build/64/FallOfDyrangor_WINDOWS.exe !obj! !linker!
	) else (
		if /I "%processor_architecture%" == "x86" (
			g++ -o build/32/FallOfDyrangor_WINDOWS.exe !obj! !linker!
		) else (
			g++ -o build/64/FallOfDyrangor_WINDOWS.exe !obj! !linker!
		)
	)
)
