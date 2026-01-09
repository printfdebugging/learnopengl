#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "window.h"
#include "logger.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"

#include <stdlib.h>

int main()
{
    struct Window *window;
    struct Mesh   *mesh;

    window = winCreate(800, 600, "OpenGL", (vec4) { 0.156f, 0.172f, 0.203f, 1.0f });
    if (!window)
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

        mesh = meshCreate();
        if (!mesh)
            return EXIT_FAILURE;

        meshLoadVertices(mesh, vertices, 4, 3 * sizeof(float));
        meshLoadColors(mesh, colors, 4, 3 * sizeof(float));
        meshLoadUV(mesh, uv, 4, 2 * sizeof(float));
        meshLoadIndices(mesh, indices, sizeof(indices), GL_UNSIGNED_INT);
    }

    {
        // SHADER
        if (meshLoadShader(mesh, ASSETS_DIR "shaders/shader.vert", ASSETS_DIR "shaders/shader.frag"))
            return EXIT_FAILURE;
    }

    {
        // TEXTURES & THEIR UNIFORMS IN THE SHADER
        mesh->textures[TEXTURE0] = malloc(sizeof(struct Texture) * 2);
        mesh->textures[TEXTURE1] = malloc(sizeof(struct Texture));

        if (!mesh->textures[TEXTURE0] || !mesh->textures[TEXTURE1])
        {
            ERROR("failed to allocate memory for textures\n");
            return EXIT_FAILURE;
        }

        {
            if (txLoadFromFile(mesh->textures[TEXTURE0], ASSETS_DIR "textures/container.jpg", "containerTexture", TEXTURE0))
                return EXIT_FAILURE;

            int faceTextureLocation = shGetUniformLocation(mesh->shader, "faceTexture");
            if (faceTextureLocation != -1)
                glUniform1i(faceTextureLocation, TEXTURE0);
        }

        {
            if (txLoadFromFile(mesh->textures[TEXTURE1], ASSETS_DIR "textures/awesomeface.png", "faceTexture", TEXTURE1))
                return EXIT_FAILURE;

            int containerTextureLocation = shGetUniformLocation(mesh->shader, "containerTexture");
            if (containerTextureLocation != -1)
                glUniform1i(containerTextureLocation, TEXTURE1);
        }
    }

    while (!winClosed(window))
    {
        winPollEvents(window);
        winProcessInput(window);
        winClearColor(window);

        {
            mat4 transform = GLM_MAT4_IDENTITY_INIT;
            glm_rotate(transform, glm_rad(90.0f), (vec3) { 0.0, 0.0, 1.0 });
            glm_scale(transform, (vec3) { 1.0, 1.0, 1.0 });

            {
                int transformLocation = shGetUniformLocation(mesh->shader, "transform");
                if (transformLocation != -1)
                    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, (float *) transform);
            }
        }

        glBindVertexArray(mesh->vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textures[0]->texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh->textures[1]->texture);

        glUseProgram(mesh->shader->program);
        glDrawElements(GL_TRIANGLES, mesh->eboCount, mesh->eboType, 0);

        winSwapBuffers(window);
    }

    winDestroy(window);
    meshDestroy(mesh);

    return 0;
}
