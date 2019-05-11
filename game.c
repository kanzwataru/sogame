#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

struct GameData {
    SDL_Window *wnd;
    SDL_Renderer *renderer;
    struct Game *game_table;
};

static struct GameData *g = NULL;

static void recompile(void) {
    puts("recompiling...");
    system("make game");
    puts("done recompiling");
}

void init(void) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Quit();
        g->game_table->running = 0;
    }

    g->wnd = SDL_CreateWindow("SOGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 200, 0);
}

int input(void) {
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        switch(e.type) {
        case SDL_QUIT:
            return 0;
        case SDL_KEYDOWN:
            if(e.key.keysym.sym == SDLK_F5)
                recompile();
            puts("down");
            break;
        }
    }

    return 1;
}

void update(float delta_time) {
}

void render(void) {
}

void quit(void) {
    if(g->wnd) {
        SDL_DestroyWindow(g->wnd);
        SDL_Quit();
    }
}

void game_library_loaded(struct Game *game, void *memory_chunk) {
    g = memory_chunk;
    g->game_table = game;

    game->init = init;
    game->input = input;
    game->update = update;
    game->render = render;
    game->quit = quit;
    game->running = 1;
}
