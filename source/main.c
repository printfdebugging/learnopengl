#include "shader.h"
#include "window.h"
#include "glad/glad.h"

#include <stdlib.h>

int main()
{
    struct window* window = window_create(800, 600, "OpenGL");
    if (!window) return EXIT_FAILURE;

    struct shader* shader = shader_create_from_file("shaders/shader.vert", "shaders/shader.frag");
    if (!shader) return EXIT_FAILURE;

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* clang-format off */
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };
    /* clang-format on */

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        MESH_ATTRIBUTE_POSITION,  // position of data in vao
        3,                        // number of elements
        GL_FLOAT,                 // type of data
        GL_FALSE,                 // whether normalized or not
        3 * sizeof(float),        // stride (offset between consecutive elements)
        (void*) 0                 // offset of the first element from the start
    );

    glEnableVertexAttribArray(MESH_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!window_closed(window))
    {
        window_poll_events(window);
        window_process_input(window);
        window_clear_color(window);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        window_swap_buffers(window);
    }

    window_destroy(window);
    return 0;
}
