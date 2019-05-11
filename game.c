#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

struct GameData {
    struct Game *game_table;
    SDL_Rect rects[4];
    SDL_Point dirs[4];
};

static struct GameData *g = NULL;

void init(void) {
        for(int i = 0; i < 4; ++i) {
        g->rects[i].x = i * 32;
        g->rects[i].y = i * 32;
        g->rects[i].w = 24;
        g->rects[i].h = 24;
        
        g->dirs[i].x = -1;
        g->dirs[i].y = 1;
    }
}

int input(void) {

}

void update(float delta_time) {
    for(int i = 0; i < 4; ++i) {
        g->rects[i].x += g->dirs[i].x;
        g->rects[i].y += g->dirs[i].y;
        
        if(g->rects[i].x + g->rects[i].w >= 320) {
            g->dirs[i].x = -1;
        }
        
        if(g->rects[i].x <= 0) {
            g->dirs[i].x = 1;
        }
        
        if(g->rects[i].y + g->rects[i].h >= 200) {
            g->dirs[i].y = -1;
        }
        
        if(g->rects[i].y <= 0) {
            g->dirs[i].y = 1;
        }
    }
}

void render(void) {  
    SDL_SetRenderDrawColor(g->game_table->api.rnd, 16, 32, 24, 255);
    SDL_RenderClear(g->game_table->api.rnd);
    
    SDL_SetRenderDrawColor(g->game_table->api.rnd, 64, 128, 192, 255);
    
    for(int i = 0; i < 4; ++i) {
        SDL_RenderFillRect(g->game_table->api.rnd, &g->rects[i]);
    }
    
    SDL_RenderPresent(g->game_table->api.rnd);
}

void quit(void) {

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
