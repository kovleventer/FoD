@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

For /f %%a in ('dir /b /s "src\*.cpp"') do (
	REM if not exist "o_win" mkdir o_win
	REM echo %%~da %%~pa %%~na.o
	set start=%%~da
	set folder=%%~pa
	set file=%%~na.o
	echo Compiling !start!!folder!!file!
	set folder=!folder:src=o_win!
	if not exist !start!!folder! mkdir !start!!folder!
	set obj=!obj!!start!!folder!!file! 
	g++ -O3 -Wall -std=c++14 -o !start!!folder!!file! -c %%a
)

echo Linking...
g++ -o FallOfDyrangor_WINDOWS.exe !obj! -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -mwindows
