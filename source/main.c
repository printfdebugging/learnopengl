#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "window.h"
#include "logger.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"

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

    {
        // MESH
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
        meshLoadIndices(data.mesh, indices, sizeof(indices), GL_UNSIGNED_INT);
    }

    {
        // SHADER
        if (meshLoadShader(
                data.mesh,
                ASSETS_DIR "shaders/shader.vert",
                ASSETS_DIR "shaders/shader.frag"
            ))
        {
            return EXIT_FAILURE;
        }
    }

    {
        // TEXTURES & THEIR UNIFORMS IN THE SHADER
        data.mesh->textures[TEXTURE0] = malloc(sizeof(struct Texture) * 2);
        data.mesh->textures[TEXTURE1] = malloc(sizeof(struct Texture));

        if (!data.mesh->textures[TEXTURE0] || !data.mesh->textures[TEXTURE1])
        {
            ERROR("failed to allocate memory for textures\n");
            return EXIT_FAILURE;
        }

        if (txLoadFromFile(
                data.mesh->textures[TEXTURE0],
                ASSETS_DIR "textures/container.jpg",
                "containerTexture",
                TEXTURE0
            ))
        {
            return EXIT_FAILURE;
        }

        {
            int faceTextureLocation = shGetUniformLocation(data.mesh->shader, "faceTexture");
            if (faceTextureLocation != -1)
                glUniform1i(faceTextureLocation, TEXTURE0);
        }

        if (txLoadFromFile(
                data.mesh->textures[TEXTURE1],
                ASSETS_DIR "textures/awesomeface.png",
                "faceTexture",
                TEXTURE1
            ))
        {
            return EXIT_FAILURE;
        }

        {
            int containerTextureLocation = shGetUniformLocation(data.mesh->shader, "containerTexture");
            if (containerTextureLocation != -1)
                glUniform1i(containerTextureLocation, TEXTURE1);
        }
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

    winPollEvents(d->window);
    winProcessInput(d->window);
    winClearColor(d->window);

    {
        mat4 transform = GLM_MAT4_IDENTITY_INIT;
        glm_rotate(transform, glm_rad(90.0f), (vec3) { 0.0, 0.0, 1.0 });
        glm_scale(transform, (vec3) { 1.0, 1.0, 1.0 });

        {
            int transformLocation = shGetUniformLocation(d->mesh->shader, "transform");
            if (transformLocation != -1)
                glUniformMatrix4fv(transformLocation, 1, GL_FALSE, (float *) transform);
        }
    }

    renderMesh(d->mesh);

    winSwapBuffers(d->window);
    winPostFrameChecks(d->window);
}
