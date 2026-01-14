#include "cglm/util.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "utils.h"
#include "window.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"

#include <stdlib.h>

int main()
{
    struct Window  *window;
    struct Mesh    *mesh;
    struct Shader  *shader;
    struct Texture *faceTexture;
    struct Texture *containerTexture;

    window = winCreate(800, 600, "OpenGL", (vec4) { 0.156f, 0.172f, 0.203f, 1.0f });
    if (!window)
        return EXIT_FAILURE;

    /* clang-format off */
    vec3 cubePositions[] = {
        { 0.0f,  0.0f,  0.0f },
        { 2.0f,  5.0f, -15.0f },
        {-1.5f, -2.2f, -2.5f },
        {-3.8f, -2.0f, -12.3f },
        { 2.4f, -0.4f, -3.5f },
        {-1.7f,  3.0f, -7.5f },
        { 1.3f, -2.0f, -2.5f },
        { 1.5f,  2.0f, -2.5f },
        { 1.5f,  0.2f, -1.5f },
        {-1.3f,  1.0f, -1.5f },
    };
    /* clang-format on */

    {
        // MESH
        /* clang-format off */
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        float uv[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,

            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,

            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f
        };
        /* clang-format on */

        mesh = meshCreate();
        if (!mesh)
            return EXIT_FAILURE;

        meshLoadVertices(mesh, vertices, 36, 3 * sizeof(float));
        // meshLoadColors(mesh, colors, 4, 3 * sizeof(float));
        meshLoadUV(mesh, uv, 36, 2 * sizeof(float));
        // meshLoadIndices(mesh, indices, sizeof(indices), GL_UNSIGNED_INT);
    }

    {
        // SHADER
        shader = shCreate();
        if (!shader)
            return EXIT_FAILURE;

        if (shLoadFromFile(shader, ASSETS_DIR "shaders/shader.vert", ASSETS_DIR "shaders/shader.frag"))
            return EXIT_FAILURE;
    }

    {
        {
            faceTexture = texCreate();
            if (!faceTexture)
                return EXIT_FAILURE;

            if (texLoadFromFile(faceTexture, ASSETS_DIR "textures/awesomeface.png"))
                return EXIT_FAILURE;

            int faceTextureLocation = shGetUniformLocation(shader, "faceTexture");
            if (faceTextureLocation != -1)
                glUniform1i(faceTextureLocation, 0);
        }

        {
            containerTexture = texCreate();
            if (!containerTexture)
                return EXIT_FAILURE;

            if (texLoadFromFile(containerTexture, ASSETS_DIR "textures/container.jpg"))
                return EXIT_FAILURE;

            int containerTextureLocation = shGetUniformLocation(shader, "containerTexture");
            if (containerTextureLocation != -1)
                glUniform1i(containerTextureLocation, 1);
        }
    }

    vec3 cpos    = { 0.0f, 0.0f, 3.0f };
    vec3 worigin = { 0.0f, 0.0f, 0.0f };
    vec3 wup     = { 0.0f, 1.0f, 0.0f };

    while (!winClosed(window))
    {
        winPollEvents(window);
        winProcessInput(window);
        winClearColor(window);

        mat4  view;
        float radius = 10.0f;
        float camx   = sin(glfwGetTime()) * radius;
        float camz   = cos(glfwGetTime()) * radius;

        cpos[0] = camx;
        cpos[2] = camz;

        glm_lookat(cpos, worigin, wup, view);

        mat4 projection;
        glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

        int viewLocation = shGetUniformLocation(shader, "view");
        if (viewLocation != -1)
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view[0]);

        int projectionLocation = shGetUniformLocation(shader, "projection");
        if (projectionLocation != -1)
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection[0]);

        glBindVertexArray(mesh->vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, containerTexture->texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, faceTexture->texture);

        glUseProgram(shader->program);

        for (unsigned int i = 0; i < 10; ++i)
        {
            {
                mat4 model = GLM_MAT4_IDENTITY_INIT;
                glm_translate(model, cubePositions[i]);
                float angle = 20.0f * (i + 1);
                glm_rotate(model, (float) glfwGetTime() * glm_rad(angle), (vec3) { 0.5f, 0.3f, 0.5f });

                int modelLocation = shGetUniformLocation(shader, "model");
                if (modelLocation != -1)
                    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model[0]);
            }

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        winSwapBuffers(window);
    }

    winDestroy(window);
    meshDestroy(mesh);
    shDestroy(shader);
    texDestroy(faceTexture);
    texDestroy(containerTexture);

    return 0;
}
