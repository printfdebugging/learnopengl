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
    /*
        remember that we draw everything with triangles,
        so specifying just four vertices to draw a square would
        be pointless.
    */
    float vertices[] = {
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    /* clang-format on */

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // is this per vao or is it something global?
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*
        NOTE: this has nothing to do with the number
        of elements in the source, `glBufferData` is there for
        that task. here we specify how each slot in the vao
        accesses it's data from a buffer.
    */
    glVertexAttribPointer(
        MESH_ATTRIBUTE_POSITION,  // position of data in vao
        3,                        // number of components per generic element
        GL_FLOAT,                 // type of data
        GL_FALSE,                 // whether normalized or not
        3 * sizeof(float),        // stride i.e. offset between consecutive generic elements
        (void*) 0                 // offset of the first element from the start
    );

    glEnableVertexAttribArray(MESH_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!window_closed(window))
    {
        window_poll_events(window);
        window_process_input(window);
        window_clear_color(window);

        // too many open questions here, the last element is indices why then when
        // i pass `indices` there, nothing is drawn. `GL_ELEMENT_ARRAY_BUFFER` might
        // be internally be the default fallback for indices but i don't see that
        // documented anywhere. maybe the specification would have answers or i should
        // read the implementation and maybe attempt to have one of my own.
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, NULL);

        window_swap_buffers(window);
    }

    window_destroy(window);
    return 0;
}
