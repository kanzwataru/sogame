.PHONY: game engine all

all: engine game

engine:
	gcc engine.c -ldl -o dllgame

game:
	touch lock
	gcc -shared -fPIC -lSDL2 game.c -o game.so
	rm lock

