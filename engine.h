#ifndef ENGINE_H
#define ENGINE_H
#include <SDL2/SDL.h>

struct API {
    SDL_Window   *wnd;
    SDL_GLContext gl_context;
};

struct Game {
    void (*init)(void);
    int (*input)(void);
    void (*update)(float);
    void (*render)(void);
    void (*quit)(void);

    struct API api;
    int running;
};

typedef int (*GameHandshakeFunction)(struct Game*, void *memory_chunk);

#endif
