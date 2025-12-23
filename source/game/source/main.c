#include "glad/glad.h"

#include "core/window.h"
#include "core/logger.h"

#include "loader/mesh.h"
#include "loader/shader.h"
#include "loader/texture.h"

#include <stdlib.h>

struct GameData
{
    struct Window*  window;
    struct Shader*  shader;
    struct Texture* texture[2];
    // TODO: remove the above since the
    //       mesh has it's shaders & textures
    struct Mesh* mesh;
};

void drawFrameCallback(void* data)
{
    struct GameData* appData = (struct GameData*) data;

    winPollEvents(appData->window);
    winProcessInput(appData->window);
    winClearColor(appData->window);

    // TODO: create a renderer which can render mesh &
    // just have renderer calls here.

    // mesh
    //  vertices & related data
    //  textures
    //  shaders <-- use the textures

    // getting this error in wasm for some reason
    // (index):1 [.WebGL-0x317c00179800] GL_INVALID_OPERATION: glDrawElements: Must have element array buffer bound.
    meshBind(appData->mesh);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    winSwapBuffers(appData->window);
    winPostFrameChecks(appData->window);
}

int main()
{
    struct GameData data;

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



    // float vertices[] = {
    //     -0.5f,  0.5f, 0.0f,
    //     -0.5f, -0.5f, 0.0f,
    //      0.5f, -0.5f, 0.0f,
    //      0.5f,  0.5f, 0.0f,
    // };
    //
    // float colors[] = {
    //     1.0f, 0.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f,
    //     0.0f, 0.0f, 1.0f,
    //     1.0f, 1.0f, 0.0f,
    // };
    //
    // float uv[] = {
    //     0.0f, 1.0f,
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    //     1.0f, 1.0f,
    // };

    int indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    /* clang-format on */

    data.mesh = meshCreate();
    meshLoadVertices(data.mesh, vertices, 4, 8 * sizeof(float));
    meshLoadColors(data.mesh, vertices + 3, 4, 8 * sizeof(float));
    meshLoadUV(data.mesh, vertices + 6, 4, 8 * sizeof(float));
    meshLoadIndices(data.mesh, indices, sizeof(indices));

    // meshLoadVertices(data.mesh, vertices, 4, 3 * sizeof(float));
    // meshLoadColors(data.mesh, colors, 4, 3 * sizeof(float));
    // meshLoadUV(data.mesh, uv, 4, 2 * sizeof(float));
    // meshLoadIndices(data.mesh, indices, sizeof(indices));

    // TODO: DESIGN: think of ways to move this into mesh. should
    // these be constructed inside a mesh or outside and then
    // assigned?
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

    meshDestroy(data.mesh);

    return 0;
}
