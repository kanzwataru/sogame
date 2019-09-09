.PHONY: game engine all

all: engine game

engine:
	gcc engine.c -ldl -lSDL2 -o sogame -Werror -Wall

game:
	touch lock
	gcc -I glad/include -shared -fPIC -lSDL2 game.c glad/src/glad.c -o game.so -Werror -Wall
	rm lock
