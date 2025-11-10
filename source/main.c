#include "shader.h"
#include "window.h"
#include "logger.h"

#include "glad/glad.h"
#include "stb_image.h"

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
        // vertices          // colors
        -0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
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
        8 * sizeof(float),        // stride i.e. offset between consecutive generic elements
        (void*) 0);               // offset of the first element from the start

    glVertexAttribPointer(
        MESH_ATTRIBUTE_COLOR,  // color attribute
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),             // stride == after what length would i find the next entry of this attribute
        (void*) (3 * sizeof(float)));  // at what index does the first element start.

    glVertexAttribPointer(
        MESH_ATTRIBUTE_UV,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*) (6 * sizeof(float)));

    glEnableVertexAttribArray(MESH_ATTRIBUTE_POSITION);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_COLOR);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_UV);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    unsigned int TEXTURE0;
    {
        glGenTextures(1, &TEXTURE0);
        glActiveTexture(GL_TEXTURE0);            // -> make this texture active
        glBindTexture(GL_TEXTURE_2D, TEXTURE0);  // -> bind the texture to a texturing target

        int width, height, nchannels;
        stbi_set_flip_vertically_on_load(GL_TRUE);
        unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nchannels, 0);
        if (!data) return EXIT_FAILURE;

        glTexImage2D(       // specify a 2D texture image
            GL_TEXTURE_2D,  // target
            0,              // level (related to mipmaps)
            GL_RGB,         // internal format
            width,
            height,
            0,       // border
            GL_RGB,  // format
            GL_UNSIGNED_BYTE,
            data  // image data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    unsigned int TEXTURE1;
    {
        glGenTextures(1, &TEXTURE1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, TEXTURE1);

        int width, height, nchannels;
        stbi_set_flip_vertically_on_load(GL_TRUE);
        unsigned char* data = stbi_load("textures/awesomeface.png", &width, &height, &nchannels, 0);
        if (!data) return EXIT_FAILURE;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    {
        {
            int texture00_location = glGetUniformLocation(shader->program, "TEXTURE0");
            if (texture00_location == -1)
            {
                ERROR("no uniform named TEXTURE0 found in shader->program\n");
                return EXIT_FAILURE;
            }
            glUniform1i(texture00_location, 0);
        }

        {
            int texture01_location = glGetUniformLocation(shader->program, "TEXTURE1");
            if (texture01_location == -1)
            {
                ERROR("no uniform named TEXTURE1 found in shader->program\n");
                return EXIT_FAILURE;
            }
            glUniform1i(texture01_location, 1);
        }
    }

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
