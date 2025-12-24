#include "glad/glad.h"

#include "core/window.h"
#include "core/logger.h"

#include "loader/mesh.h"
#include "loader/shader.h"
#include "loader/texture.h"

#include <stdlib.h>

struct GameData
{
    struct Window *window;
    struct Mesh   *mesh;
};

DrawFrameCallback drawFrameCallback;

int main()
{
    struct GameData data;

    data.window = winCreate(966, 602, "OpenGL", (vec4) { 0.156f, 0.172f, 0.203f, 1.0f });
    if (!data.window)
        return EXIT_FAILURE;

    /* clang-format off */
    float vertices[] = {
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
    };

    float uv[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    int indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    /* clang-format on */

    data.mesh = meshCreate();
    if (!data.mesh)
        return EXIT_FAILURE;

    meshLoadVertices(data.mesh, vertices, 4, 3 * sizeof(float));
    meshLoadColors(data.mesh, colors, 4, 3 * sizeof(float));
    meshLoadUV(data.mesh, uv, 4, 2 * sizeof(float));
    meshLoadIndices(data.mesh, indices, sizeof(indices));

    meshLoadTexture(
        data.mesh,
        (char *) ASSETS_DIR "textures/container.jpg",
        "containerTexture",
        TEXTURE0
    );

    meshLoadTexture(
        data.mesh,
        (char *) ASSETS_DIR "textures/awesomeface.png",
        "faceTexture",
        TEXTURE1
    );

    meshLoadShader(
        data.mesh,
        ASSETS_DIR "shaders/shader.vert",
        ASSETS_DIR "shaders/shader.frag"
    );

    winRegisterDrawFrameCallback(data.window, drawFrameCallback);
    winFireMainLoop(data.window, (void *) &data);

    winDestroy(data.window);
    meshDestroy(data.mesh);

    return 0;
}

void drawFrameCallback(void *data)
{
    struct GameData *appData = (struct GameData *) data;

    winPollEvents(appData->window);
    winProcessInput(appData->window);
    winClearColor(appData->window);

    // TODO: create a renderer which can render mesh &
    // just have renderer calls here.

    meshBind(appData->mesh);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    winSwapBuffers(appData->window);
    winPostFrameChecks(appData->window);
}
