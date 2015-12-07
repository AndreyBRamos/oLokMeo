all:  execute

execute: compile
	./a.out 

compile: headerfile
	g++ *.cpp -lGL -lGLU -lglut -Wall

headerfile:
	g++ tga.h camera.h globals.h moon.h nave.h planet.h solarsystem.h  -Wall
