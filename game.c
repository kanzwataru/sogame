#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define RECT_SPEED  2
#define RECT_COUNT  4

struct GameData {
    struct Game *game_table;
    SDL_Rect rects[RECT_COUNT];
    SDL_Point dirs[RECT_COUNT];
};

static struct GameData *g = NULL;

void init(void) {
    for(int i = 0; i < 4; ++i) {
        g->rects[i].x = i * 64;
        g->rects[i].y = i * 64;
        g->rects[i].w = 48;
        g->rects[i].h = 48;
        
        g->dirs[i].x = -1;
        g->dirs[i].y = 1;
    }
}

int input(void) {

}

void update(float delta_time) {
    for(int i = 0; i < RECT_COUNT; ++i) {
        g->rects[i].x += g->dirs[i].x * RECT_SPEED;
        g->rects[i].y += g->dirs[i].y * RECT_SPEED;
        
        if(g->rects[i].x + g->rects[i].w >= 640) {
            g->dirs[i].x = -1;
        }
        
        if(g->rects[i].x <= 0) {
            g->dirs[i].x = 1;
        }
        
        if(g->rects[i].y + g->rects[i].h >= 480) {
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
    
    for(int i = 0; i < RECT_COUNT; ++i) {
        SDL_RenderFillRect(g->game_table->api.rnd, &g->rects[i]);
    }
    
    SDL_RenderPresent(g->game_table->api.rnd);
}

void quit(void) {

}

void game_library_loaded(struct Game *game, void *memory_chunk) {
    srand((size_t)memory_chunk * (size_t)game);
    g = memory_chunk;
    g->game_table = game;

    game->init = init;
    game->input = input;
    game->update = update;
    game->render = render;
    game->quit = quit;
    game->running = 1;
}
