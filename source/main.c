#include "cglm/struct.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "utils.h"
#include "window.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"

#include <stdlib.h>

struct Window  *window;
struct Mesh    *mesh;
struct Shader  *shader;
struct Texture *faceTexture;
struct Texture *containerTexture;

vec3s cameraPos   = { 0.0f, 0.0f, 3.0f };
vec3s cameraFront = { 0.0f, 0.0f, -1.0f };
vec3s cameraUp    = { 0.0f, 1.0f, 0.0f };

float deltaTime  = 0.0f;
float lastFrame  = 0.0f;
float yaw        = -90.0f;
float pitch      = 0.0f;
float lastX      = 400;
float lastY      = 300;
bool  firstMouse = true;
float fov        = 45.0f;

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
    window = winCreate(800, 600, "OpenGL", (vec4s) { 0.156f, 0.172f, 0.203f, 1.0f });
    if (!window)
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

    while (!winClosed(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        winPollEvents(window);
        winProcessInput(window);
        processInput(window);
        winClearColor(window);

        vec3s sum = glms_vec3_add(cameraPos, cameraFront);

        mat4s view       = glms_lookat(cameraPos, sum, cameraUp);
        mat4s projection = glms_perspective(glm_rad(fov), 800.0f / 600.0f, 0.1f, 100.0f);

        int viewLocation = shGetUniformLocation(shader, "view");
        if (viewLocation != -1)
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view.col[0].raw[0]);

        int projectionLocation = shGetUniformLocation(shader, "projection");
        if (projectionLocation != -1)
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection.col[0].raw[0]);

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
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS)
    {
        vec3s mul = glms_vec3_scale(cameraFront, cameraSpeed);
        cameraPos = glms_vec3_add(cameraPos, mul);
    }

    if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS)
    {
        vec3s mul = glms_vec3_scale(cameraFront, cameraSpeed);
        cameraPos = glms_vec3_sub(cameraPos, mul);
    }

    if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS)
    {
        vec3s cross = glms_cross(cameraFront, cameraUp);
        cross       = glms_normalize(cross);

        vec3s mul = glms_vec3_scale(cross, cameraSpeed);
        cameraPos = glms_vec3_sub(cameraPos, mul);
    }

    if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS)
    {
        vec3s cross = glms_cross(cameraFront, cameraUp);
        cross       = glms_normalize(cross);
        vec3s mul   = glms_vec3_scale(cross, cameraSpeed);
        cameraPos   = glms_vec3_add(cameraPos, mul);
    }
}

void mouseCallback(GLFWwindow *window,
                   double      posX,
                   double      posY)
{
    if (firstMouse)
    {
        lastX      = posX;
        lastY      = posY;
        firstMouse = false;
    }

    float offsetX = posX - lastX;
    float offsetY = posY - lastY;

    lastX = posX;
    lastY = posY;

    const float sensitivity = 0.1f;
    offsetX *= sensitivity;
    offsetY *= sensitivity;

    yaw += offsetX;
    pitch += offsetY;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3s direction;
    direction.x = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    direction.y = sin(glm_rad(pitch));
    direction.z = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    direction   = glms_normalize(direction);
    cameraFront = direction;
}

void scrollCallback(GLFWwindow *window,
                    double      offsetX,
                    double      offsetY)
{
    fov -= (float) offsetY;
    if (fov < 1.0)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
