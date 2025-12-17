#include "loader/shader.h"
#include "glad/glad.h"
#include "core/window.h"

#ifdef EMSCRIPTEN
    #include <emscripten/version.h>
    #include <emscripten/emscripten.h>
#endif

#include <stdlib.h>

unsigned int   vao;
unsigned int   vbo;
struct Shader* shader;
struct Window* window;

// TODO: create winRegisterDrawFrameCallback ... and register it there
// create a typedef for the function signature
void frame()
{
    winPollEvents(window);
    winProcessInput(window);
    winClearColor(window);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    winSwapBuffers(window);
}

int main()
{
    window = winCreate(966, 602, "OpenGL", (vec4) { 0.2f, 0.3f, 0.3f, 1.0f });
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

    shader = shCreateFromFile(
        ASSETS_DIR "shaders/shader.vert",
        ASSETS_DIR "shaders/shader.frag"
    );

    // TODO: create a winStartMainLoop() function which abstracts away
    // these platform specific loops
#ifdef EMSCRIPTEN
    printf(
        "emscripten: v%d.%d.%d\n",
        __EMSCRIPTEN_major__,
        __EMSCRIPTEN_minor__,
        __EMSCRIPTEN_tiny__
    );
    emscripten_set_main_loop(frame, 0, false);
#else
    while (!winClosed(window))
    {
        frame();
    }
#endif

    winDestroy(window);
    return 0;
}
