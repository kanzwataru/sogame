#ifndef ENGINE_H
#define ENGINE_H

struct Game {
    void (*init)(void);
    int (*input)(void);
    void (*update)(float);
    void (*render)(void);
    void (*quit)(void);
    int running;
};

typedef int (*GameHandshakeFunction)(struct Game*, void *memory_chunk);

#endif
