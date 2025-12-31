#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "core/window.h"
#include "core/logger.h"

#include "loader/mesh.h"
#include "loader/shader.h"
#include "loader/texture.h"

#include "renderer/renderer.h"

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

    data.window = winCreate(800, 600, "OpenGL", (vec4) { 0.156f, 0.172f, 0.203f, 1.0f });
    if (!data.window)
        return EXIT_FAILURE;

    /* clang-format off */
    float vertices[] = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
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
    meshLoadIndices(data.mesh, indices, sizeof(indices), GL_UNSIGNED_INT);

    if (meshLoadShader(
            data.mesh,
            ASSETS_DIR "shaders/shader.vert",
            ASSETS_DIR "shaders/shader.frag"
        ))
    {
        return EXIT_FAILURE;
    }

    winRegisterDrawFrameCallback(data.window, drawFrameCallback);
    winFireMainLoop(data.window, (void *) &data);

    winDestroy(data.window);
    meshDestroy(data.mesh);

    return 0;
}

void drawFrameCallback(void *data)
{
    struct GameData *d = (struct GameData *) data;

    // {
    //     double x, y;
    //     glfwGetCursorPos(d->window->window, &x, &y);
    //     float position[2] = { x, y };
    //
    //     int mousePositionLocation = shGetUniformLocation(d->mesh->shader, "mousePosition");
    //     if (mousePositionLocation != -1)
    //         glUniform2f(mousePositionLocation, position[0], position[1]);
    // }

    {
        int w, h;
        glfwGetWindowSize(d->window->window, &w, &h);
        float dimensions[2] = { w, h };

        int canvasDimensionsLocation = shGetUniformLocation(d->mesh->shader, "canvasDimensions");
        if (canvasDimensionsLocation != -1)
            glUniform2f(canvasDimensionsLocation, dimensions[0], dimensions[1]);
    }

    winPollEvents(d->window);
    winProcessInput(d->window);
    winClearColor(d->window);

    renderMesh(d->mesh);

    winSwapBuffers(d->window);
    winPostFrameChecks(d->window);
}
