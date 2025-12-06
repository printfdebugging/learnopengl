#include "shader.h"
#include "window.h"

#include "glad/glad.h"
#ifdef EMSCRIPTEN
#include <emscripten/version.h>
#include <emscripten/emscripten.h>
#endif

#include <stdlib.h>

unsigned int   vao;
unsigned int   vbo;
struct shader* shader;
struct window* window;

void frame()
{
    window_poll_events(window);
    window_process_input(window);
    window_clear_color(window);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    window_swap_buffers(window);
}

int main()
{
    window = window_create(
        800,
        600,
        "OpenGL",
        (vec4) { 0.2f, 0.3f, 0.3f, 1.0f }
    );

    if (!window)
        return EXIT_FAILURE;

    /* clang-format off */
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    /* clang-format on */

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*) 0
    );

    glEnableVertexAttribArray(0);

    shader = shader_create_from_file(
        ASSET_DIR "shaders/shader.vert",
        ASSET_DIR "shaders/shader.frag"
    );

#ifdef EMSCRIPTEN
    printf("emscripten: v%d.%d.%d\n", __EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__);
    emscripten_set_main_loop(frame, 0, false);
#else
    while (!window_closed(window))
    {
        frame();
    }
#endif

    window_destroy(window);
    return 0;
}
