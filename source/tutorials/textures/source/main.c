#include "loader/shader.h"
#include "loader/texture.h"
#include "glad/glad.h"
#include "core/window.h"
#include "core/logger.h"

#include <stdlib.h>

struct AppData
{
    struct Window*  window;
    struct Shader*  shader;
    struct Texture* texture[2];
};

void drawFrameCallback(void* data)
{
    struct AppData* appData = (struct AppData*) data;

    winPollEvents(appData->window);
    winProcessInput(appData->window);
    winClearColor(appData->window);

    // getting this error in wasm for some reason
    // (index):1 [.WebGL-0x317c00179800] GL_INVALID_OPERATION: glDrawElements: Must have element array buffer bound.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    winSwapBuffers(appData->window);
    winPostFrameChecks(appData->window);
}

int main()
{
    struct AppData data;

    data.window = winCreate(966, 602, "OpenGL", (vec4) { 0.156f, 0.172f, 0.203f, 1.0f });
    if (!data.window)
        return EXIT_FAILURE;

    /* clang-format off */
    float vertices[] = {
        // vertices             // colors          // uv
        -0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    };

    int indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    /* clang-format on */

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        MESH_ATTRIBUTE_POSITION,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*) 0
    );
    glEnableVertexAttribArray(MESH_ATTRIBUTE_POSITION);

    glVertexAttribPointer(
        MESH_ATTRIBUTE_COLOR,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*) (3 * sizeof(float))
    );
    glEnableVertexAttribArray(MESH_ATTRIBUTE_COLOR);

    glVertexAttribPointer(
        MESH_ATTRIBUTE_UV,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*) (6 * sizeof(float))
    );
    glEnableVertexAttribArray(MESH_ATTRIBUTE_UV);

    data.shader = shCreateFromFile(
        ASSETS_DIR "shaders/shader.vert",
        ASSETS_DIR "shaders/shader.frag"
    );

    data.texture[0] = txLoadFromFile(
        (char*) ASSETS_DIR "textures/container.jpg",
        "containerTexture",
        GL_TEXTURE0
    );

    data.texture[1] = txLoadFromFile(
        (char*) ASSETS_DIR "textures/awesomeface.png",
        "faceTexture",
        GL_TEXTURE1
    );

    // TODO: would have been great if i could move this to
    // the texture loader, but seems like that's a bad idea.
    shUniform1i(data.shader, "containerTexture", 0);
    shUniform1i(data.shader, "faceTexture", 1);

    winRegisterDrawFrameCallback(data.window, drawFrameCallback);
    winFireMainLoop(data.window, (void*) &data);

    winDestroy(data.window);
    txDestroy(data.texture[0]);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);

    return 0;
}
