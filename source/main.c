#include "GLFW/glfw3.h"
#include "cglm/struct.h"
#include "cglm/struct/affine.h"
#include "glad/glad.h"

#include "camera.h"
#include "mesh.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "utils.h"
#include "window.h"

#include <stdlib.h>

struct window *window;
struct camera *camera;

struct mesh *cube_mesh;
struct mesh *lines_mesh;
struct mesh *light_mesh;

struct shader *cube_shader;
struct shader *lines_shader;
struct shader *light_shader;

struct texture *face_texture;
struct texture *container_texture;

float delta_time = 0.0f;
float last_frame = 0.0f;
const float WIDTH = 1920.0f;
const float HEIGHT = 1080.0f;

void process_input(struct window *window);

void mouse_callback(GLFWwindow *window, double posX, double posY);

void scroll_callback(GLFWwindow *window, double offsetX, double offsetY);

int main()
{
    window =
        window_create(WIDTH, HEIGHT, "OpenGL", (vec4s) { 0.156f, 0.172f, 0.203f, 1.0f });
    if (!window)
        return EXIT_FAILURE;

    camera = camera_create();
    if (!camera)
        return EXIT_FAILURE;

    glfwSetCursorPosCallback(window->window, mouse_callback);
    glfwSetScrollCallback(window->window, scroll_callback);

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

        cube_mesh = mesh_create();
        if (!cube_mesh)
            return EXIT_FAILURE;

        mesh_load_vertices(cube_mesh, vertices, 36, 3 * sizeof(float));
        // mesh_load_colors(mesh, colors, 4, 3 * sizeof(float));
        mesh_load_uv(cube_mesh, uv, 36, 2 * sizeof(float));
        // mesh_load_indices(mesh, indices, sizeof(indices), GL_UNSIGNED_INT);

        light_mesh = mesh_create();
        if (!cube_mesh)
            return EXIT_FAILURE;

        mesh_load_vertices(light_mesh, vertices, 36, 3 * sizeof(float));
        mesh_load_uv(light_mesh, uv, 36, 2 * sizeof(float));
    }

    {
        // SHADER
        cube_shader = shader_create();
        if (!cube_shader)
            return EXIT_FAILURE;

        if (shader_load_from_file(cube_shader, ASSETS_DIR "shaders/cube.vert", ASSETS_DIR "shaders/cube.frag"))
            return EXIT_FAILURE;
    }

    {
        const int AXES = 2;
        const int LINES_PER_AXIS = 51;
        const int POINTS_PER_LINE = 2;
        const int FLOATS_PER_POINT = 3;
        const int count = AXES * LINES_PER_AXIS * POINTS_PER_LINE;

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

        lines_mesh = mesh_create();
        mesh_load_vertices(lines_mesh, &vertices[0][0][0][0], count, 3 * sizeof(float));
    }

    {
        lines_shader = shader_create();
        if (!lines_shader)
            return EXIT_FAILURE;
        if (shader_load_from_file(lines_shader, ASSETS_DIR "shaders/lines.vert", ASSETS_DIR "shaders/lines.frag"))
            return EXIT_FAILURE;
    }

    {
        light_shader = shader_create();
        if (!light_shader)
            return EXIT_FAILURE;
        if (shader_load_from_file(light_shader, ASSETS_DIR "shaders/light.vert", ASSETS_DIR "shaders/light.frag"))
            return EXIT_FAILURE;
    }

    {
        face_texture = texture_create();
        if (!face_texture)
            return EXIT_FAILURE;

        if (texture_load_from_file(face_texture, ASSETS_DIR "textures/awesomeface.png"))
            return EXIT_FAILURE;

        int faceTextureLocation = shader_get_uniform_location(cube_shader, "faceTexture");
        if (faceTextureLocation != -1)
            glUniform1i(faceTextureLocation, 0);
    }

    {
        container_texture = texture_create();
        if (!container_texture)
            return EXIT_FAILURE;

        if (texture_load_from_file(container_texture, ASSETS_DIR "textures/container.jpg"))
            return EXIT_FAILURE;

        int containerTextureLocation =
            shader_get_uniform_location(cube_shader, "containerTexture");
        if (containerTextureLocation != -1)
            glUniform1i(containerTextureLocation, 1);
    }

    while (!window_close(window))
    {
        float currentFrame = glfwGetTime();
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;

        window_poll_events(window);
        window_process_input(window);
        process_input(window);
        window_clear_color(window);

        vec3s sum = glms_vec3_add(camera->position, camera->front);
        mat4s view = glms_lookat(camera->position, sum, camera->up);
        mat4s projection =
            glms_perspective(glm_rad(camera->fov), WIDTH / HEIGHT, 0.1f, 100.0f);

        {
            int viewLocation = shader_get_uniform_location(lines_shader, "view");
            if (viewLocation != -1)
                glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view.col[0].raw[0]);

            int projectionLocation = shader_get_uniform_location(lines_shader, "projection");
            if (projectionLocation != -1)
                glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection.col[0].raw[0]);
        }

        {
            int viewLocation = shader_get_uniform_location(cube_shader, "view");
            if (viewLocation != -1)
                glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view.col[0].raw[0]);

            int projectionLocation = shader_get_uniform_location(cube_shader, "projection");
            if (projectionLocation != -1)
                glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection.col[0].raw[0]);
        }

        {
            int viewLocation = shader_get_uniform_location(light_shader, "view");
            if (viewLocation != -1)
                glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view.col[0].raw[0]);

            int projectionLocation = shader_get_uniform_location(light_shader, "projection");
            if (projectionLocation != -1)
                glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection.col[0].raw[0]);
        }

        float angle = 0.0f;
        vec3s meshPosition = { 0.0f, 0.0f, 0.0f };

        {
            // cube
            glBindVertexArray(cube_mesh->vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, container_texture->texture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, face_texture->texture);
            glUseProgram(cube_shader->program);

            mat4s model = glms_mat4_identity();
            model = glms_translate(model, meshPosition);
            model = glms_rotate(model, (float) glfwGetTime() * glm_rad(angle), (vec3s) { 0.5f, 0.3f, 0.5f });

            int modelLocation = shader_get_uniform_location(cube_shader, "model");
            if (modelLocation != -1)
                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model.col[0].raw[0]);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        {
            // axes
            glBindVertexArray(lines_mesh->vao);
            glUseProgram(lines_shader->program);
            glDrawArrays(GL_LINES, 0, lines_mesh->vertex_count);
        }

        {
            // light
            vec3s lightPosition = { 1.2f, 1.0f, 2.0f };
            vec3s lightColor = { 1.0f, 1.0f, 1.0f };
            vec3s objectColor = { 1.0f, 0.5f, 0.31f };
            vec3s scale = { 0.2f, 0.2f, 0.2f };
            vec3s axisOfRotation = { 0.5f, 0.3f, 0.5f };

            mat4s modelLight = glms_mat4_identity();
            modelLight = glms_translate(modelLight, lightPosition);
            modelLight = glms_rotate(
                modelLight, (float) glfwGetTime() * glm_rad(angle), axisOfRotation
            );
            modelLight = glms_scale(modelLight, scale);

            int lightLocation = shader_get_uniform_location(light_shader, "model");
            if (lightLocation != -1)
                glUniformMatrix4fv(lightLocation, 1, GL_FALSE, &modelLight.col[0].raw[0]);

            glBindVertexArray(light_mesh->vao);
            glUseProgram(light_shader->program);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        window_swap_buffers(window);
    }

    window_destroy(window);
    mesh_destroy(cube_mesh);
    shader_destroy(cube_shader);
    texture_destroy(face_texture);
    texture_destroy(container_texture);

    return 0;
}

void process_input(struct window *window)
{
    if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS)
        camera_process_keyboard(camera, CAMERA_DIRECTION_FORWARD, delta_time);

    if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS)
        camera_process_keyboard(camera, CAMERA_DIRECTION_BACKWARD, delta_time);

    if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS)
        camera_process_keyboard(camera, CAMERA_DIRECTION_LEFT, delta_time);

    if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS)
        camera_process_keyboard(camera, CAMERA_DIRECTION_RIGHT, delta_time);
}

void mouse_callback(GLFWwindow *window, double posX, double posY)
{
    camera_process_mouse_movement(camera, (float) posX, (float) posY);
}

void scroll_callback(GLFWwindow *window, double offsetX, double offsetY)
{
    camera_process_mouse_scroll(camera, (float) offsetY);
}
