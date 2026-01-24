#include "cglm/struct.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "utils.h"
#include "window.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"

#include <stdlib.h>

struct Window  *window;
struct Mesh    *mesh;
struct Mesh    *lines;
struct Shader  *linesShader;
struct Shader  *shader;
struct Texture *faceTexture;
struct Texture *containerTexture;
struct Camera  *camera;

float       deltaTime = 0.0f;
float       lastFrame = 0.0f;
const float WIDTH     = 1920.0f;
const float HEIGHT    = 1080.0f;

void processInput(struct Window *window);

void mouseCallback(
    GLFWwindow *window,
    double      posX,
    double      posY
);

void scrollCallback(
    GLFWwindow *window,
    double      offsetX,
    double      offsetY
);

int main()
{
    window = winCreate(WIDTH, HEIGHT, "OpenGL", (vec4s) { 0.156f, 0.172f, 0.203f, 1.0f });
    if (!window)
        return EXIT_FAILURE;

    camera = cameraCreate();
    if (!camera)
        return EXIT_FAILURE;

    glfwSetCursorPosCallback(window->window, mouseCallback);
    glfwSetScrollCallback(window->window, scrollCallback);

    /* clang-format off */
    vec3s cubePositions[] = {
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

        if (shLoadFromFile(shader, ASSETS_DIR "shaders/cube.vert", ASSETS_DIR "shaders/cube.frag"))
            return EXIT_FAILURE;
    }

    {
        const int AXES             = 2;
        const int LINES_PER_AXIS   = 51;
        const int POINTS_PER_LINE  = 2;
        const int FLOATS_PER_POINT = 3;
        const int count            = AXES * LINES_PER_AXIS * POINTS_PER_LINE;

        float vertices[AXES][LINES_PER_AXIS][POINTS_PER_LINE][FLOATS_PER_POINT] = {};

        for (int z = -25; z <= 25; ++z)
        {
            vertices[0][z + 25][0][0] = -25.0f;
            vertices[0][z + 25][0][1] = 0.0f;
            vertices[0][z + 25][0][2] = (float) z;

            vertices[0][z + 25][1][0] = 25.0f;
            vertices[0][z + 25][1][1] = 0.0f;
            vertices[0][z + 25][1][2] = (float) z;
        }

        for (int x = -25; x <= 25; ++x)
        {
            vertices[1][x + 25][0][0] = (float) x;
            vertices[1][x + 25][0][1] = 0.0f;
            vertices[1][x + 25][0][2] = -25.0f;

            vertices[1][x + 25][1][0] = (float) x;
            vertices[1][x + 25][1][1] = 0.0f;
            vertices[1][x + 25][1][2] = 25.0f;
        }

        // TODO: create vertices using loops

        lines = meshCreate();
        meshLoadVertices(lines, &vertices[0][0][0][0], count, 3 * sizeof(float));
    }

    {
        linesShader = shCreate();
        if (!linesShader)
            return EXIT_FAILURE;

        if (shLoadFromFile(linesShader, ASSETS_DIR "shaders/lines.vert", ASSETS_DIR "shaders/lines.frag"))
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

    while (!winClosed(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        winPollEvents(window);
        winProcessInput(window);
        processInput(window);
        winClearColor(window);

        vec3s sum = glms_vec3_add(camera->position, camera->front);

        mat4s view       = glms_lookat(camera->position, sum, camera->up);
        mat4s projection = glms_perspective(glm_rad(camera->fov), WIDTH / HEIGHT, 0.1f, 100.0f);

        {
            int viewLocation = shGetUniformLocation(linesShader, "view");
            if (viewLocation != -1)
                glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view.col[0].raw[0]);

            int projectionLocation = shGetUniformLocation(linesShader, "projection");
            if (projectionLocation != -1)
                glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection.col[0].raw[0]);
        }

        {
            int viewLocation = shGetUniformLocation(shader, "view");
            if (viewLocation != -1)
                glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view.col[0].raw[0]);

            int projectionLocation = shGetUniformLocation(shader, "projection");
            if (projectionLocation != -1)
                glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection.col[0].raw[0]);
        }

        glBindVertexArray(lines->vao);
        glUseProgram(linesShader->program);
        glDrawArrays(GL_LINES, 0, lines->vertexCount);

        glBindVertexArray(mesh->vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, containerTexture->texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, faceTexture->texture);
        glUseProgram(shader->program);

        for (unsigned int i = 0; i < 10; ++i)
        {
            {
                float angle = 20.0f * (i + 1);

                mat4s model = glms_mat4_identity();
                model       = glms_translate(model, cubePositions[i]);
                model       = glms_rotate(model, (float) glfwGetTime() * glm_rad(angle), (vec3s) { 0.5f, 0.3f, 0.5f });

                int modelLocation = shGetUniformLocation(shader, "model");
                if (modelLocation != -1)
                    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model.col[0].raw[0]);
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

void processInput(struct Window *window)
{
    if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS)
        cameraProcessKeyboard(camera, CAMERA_DIRECTION_FORWARD, deltaTime);

    if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS)
        cameraProcessKeyboard(camera, CAMERA_DIRECTION_BACKWARD, deltaTime);

    if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS)
        cameraProcessKeyboard(camera, CAMERA_DIRECTION_LEFT, deltaTime);

    if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS)
        cameraProcessKeyboard(camera, CAMERA_DIRECTION_RIGHT, deltaTime);
}

void mouseCallback(GLFWwindow *window,
                   double      posX,
                   double      posY)
{
    cameraProcessMouseMovement(camera, (float) posX, (float) posY);
}

void scrollCallback(GLFWwindow *window,
                    double      offsetX,
                    double      offsetY)
{
    cameraProcessMouseScroll(camera, (float) offsetY);
}
