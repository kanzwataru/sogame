#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>

struct GameData {
    struct Game *game_table;
};

static struct GameData *g = NULL;

static const char * const vtx_shader_src =
"#version 330 core\n"
"layout (location = 0) in vec3 in_pos;\n"
"void main() {\n"
"   gl_Position = vec4(in_pos.xyz, 1.0f);\n"
"}\n\0";

static const char * const frg_shader_src =
"#version 330 core\n"
"out vec4 frag_col;"
"void main()\n"
"{\n"
"   frag_col = vec4(0.6, 0.4f, 0.1f, 1.0f);\n"
"}\n\0";

static const float tri[] = {
    -0.5, -0.5, 0.0f,
     0.5, -0.5, 0.0f,
     0.0,  0.5, 0.0f
};

static void check_shader_compilation(GLuint shader)
{
    GLint status;
    char  msg_buf[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE) {
        glGetShaderInfoLog(shader, 512, NULL, msg_buf);
        fprintf(stderr, "*** SHADER COMPILATION FAILED ***\n, %s\n\n", msg_buf);
    }
}

static void check_program_compilation(GLuint program)
{
    GLint status;
    char  msg_buf[512];

    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if(status != GL_TRUE) {
        glGetProgramInfoLog(program, 512, NULL, msg_buf);
        fprintf(stderr, "*** PROGRAM COMPILATION FAILED ***\n, %s\n\n", msg_buf);
    }
}

#define check_gl_error()    _check_gl_error(__FILE__, __LINE__)
static int _check_gl_error(const char *file, int line)
{
    GLenum err = 0;
    while((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "%s:%d -> OpenGL Error: %04x\n", file, line, err);
    }

    return err == 0;
}

void init(void) {
    glViewport(0, 0, 640, 480);
}

int input(void) {
    return 1;
}

void update(float delta_time) {
}

void render(void) {
    int vtx_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtx_shader, 1, &vtx_shader_src, NULL);
    glCompileShader(vtx_shader);
    check_shader_compilation(vtx_shader);

    int frg_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frg_shader, 1, &frg_shader_src, NULL);
    glCompileShader(frg_shader);
    check_shader_compilation(frg_shader);

    int shader = glCreateProgram();
    glAttachShader(shader, vtx_shader);
    glAttachShader(shader, frg_shader);
    glLinkProgram(shader);
    check_program_compilation(shader);
    glUseProgram(shader);

    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glClearColor(0.15f, 0.15f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    check_gl_error();
    SDL_GL_SwapWindow(g->game_table->api.wnd);

    glUseProgram(0);
    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);
    glDeleteShader(shader);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
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

    gladLoadGLLoader(SDL_GL_GetProcAddress);
}
