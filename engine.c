#include "engine.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <SDL2/SDL.h>

#define MEGABYTES(x) ((x) * 1000000)

static void *memory = NULL;
static void *dyn_handle = NULL;
static struct Game game_table;
static time_t library_time;

static void recompile(void) {
    puts("recompiling...");
    system("make game");
    puts("done recompiling");
}

void sdl_init(void) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Quit();
        exit(2);
    }
    
    game_table.api.wnd = SDL_CreateWindow("SOGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    game_table.api.rnd = SDL_CreateRenderer(game_table.api.wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

int sdl_handle_events(void) {
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

void sdl_quit(void) {
    if(game_table.api.wnd) {
        SDL_DestroyWindow(game_table.api.wnd);
        SDL_DestroyRenderer(game_table.api.rnd);
        SDL_Quit();
    }
}

void load_game(void) {
    char *error;
    struct stat attr;

    stat("game.so", &attr);
    library_time = attr.st_mtime;

    if(dyn_handle) {
        dlclose(dyn_handle);
    }

    if(system("cp game.so game.running.so") != 0) {
        exit(2);
    };

    dyn_handle = dlopen("./game.running.so", RTLD_LAZY);
    if(!dyn_handle) {
        fputs(dlerror(), stderr);
        exit(1);
    }
    
    GameHandshakeFunction handshake = dlsym(dyn_handle, "game_library_loaded");
    if((error = dlerror()) != NULL) {
        fputs(error, stderr);
        exit(1);
    }
    
    handshake(&game_table, memory);
}

void ensure_game_latest(void) {
    struct stat attr;
    
    if(0 == access("lock", F_OK)) {
        return;
    }

    stat("game.so", &attr);

    if(attr.st_mtime > library_time) {
        puts("game needs reloading");
        library_time = attr.st_mtime;
        load_game();
    }
}

void game_loop(void) {
    game_table.init();
    
    while(sdl_handle_events() && game_table.running) {
        ensure_game_latest();

        game_table.input();
        game_table.update(1.0f);
        game_table.render();
    }

    game_table.quit();
}

void cleanup(void) {
    free(memory);

    if(dyn_handle) {
        dlclose(dyn_handle);
        system("rm game.running.so");
    }
}

int main(void) {
    char *error;
    
    memory = malloc(MEGABYTES(24));
    assert(memory);
    
    sdl_init();
    
    atexit(cleanup);
    load_game();
    game_loop();
    
    return 0;
}
