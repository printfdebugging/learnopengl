#include "loader/shader.h"
#include "glad/glad.h"
#include "core/window.h"

#include <stdlib.h>

struct AppData
{
    struct Window* window;
    struct Shader* shader;
};

void drawFrameCallback(void* data)
{
    struct AppData* appData = (struct AppData*) data;

    winPollEvents(appData->window);
    winProcessInput(appData->window);
    winClearColor(appData->window);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    winSwapBuffers(appData->window);
    winPostFrameChecks(appData->window);
}

int main()
{
    struct AppData data;

    data.window = winCreate(966, 602, "OpenGL", (vec4) { 0.2f, 0.3f, 0.3f, 1.0f });
    if (!data.window)
        return EXIT_FAILURE;

    /* clang-format off */
    float vertices[] = {
        // vertices             // colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,
    };
    /* clang-format on */

    unsigned int vao;
    unsigned int vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        MESH_ATTRIBUTE_POSITION,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*) 0
    );

    glVertexAttribPointer(
        MESH_ATTRIBUTE_COLOR,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*) (3 * sizeof(float))
    );

    data.shader = shCreateFromFile(
        ASSETS_DIR "shaders/shader.vert",
        ASSETS_DIR "shaders/shader.frag"
    );

    winRegisterDrawFrameCallback(data.window, drawFrameCallback);
    winFireMainLoop(data.window, (void*) &data);

    winDestroy(data.window);

    return 0;
}
