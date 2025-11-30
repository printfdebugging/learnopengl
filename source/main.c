#include "shader.h"
#include "window.h"
#include "logger.h"

#include "glad/glad.h"
#include "cglm/cglm.h"
#include "stb_image.h"

#include <stdlib.h>

int main()
{
    struct window* window = window_create(800, 600, "OpenGL");

    if (!window)
        return EXIT_FAILURE;

    struct shader* shader = shader_create_from_file(
        "shaders/shader.vert",
        "shaders/shader.frag"
    );

    if (!shader)
        return EXIT_FAILURE;

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* clang-format off */
    /*
        remember that we draw everything with triangles,
        so specifying just four vertices to draw a square would
        be pointless.
    */

    /*
     * also remember that opengl operates in 3d
     * space, so using vec2 won't do, we need
     * to use vec3 with a dummy z coordinate for
     * 2d.
     */

    /*
        here we are mapping the texture to the drawn triangles,
        then how would i specify the texture coordinates outside
        the texture coordinate range? => i see. so if i keep the
        coordinates as (1, smth) or (smth, 1), then the texture
        maps fully to the square, if i make it 0.5, then only
        a quarter is mapped, but if i make it 2.0, suddenly
        the texture is shrunk and then the question comes,
        what to do with the rest of the space, repeat or clamp?
    */

    float vertices[] = {
        // vertices          // colors
        -0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f,  1.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
         0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f,  0.0f,
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   1.0f,  1.0f,
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
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

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
        (void*) 0                 // offset of the first element from the start
    );

    glVertexAttribPointer(
        MESH_ATTRIBUTE_COLOR,  // color attribute
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),           // stride == after what length would i find the next entry of this attribute
        (void*) (3 * sizeof(float))  // at what index does the first element start.
    );

    glVertexAttribPointer(
        MESH_ATTRIBUTE_UV,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*) (6 * sizeof(float))
    );

    glEnableVertexAttribArray(MESH_ATTRIBUTE_POSITION);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_COLOR);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_UV);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int TEXTURE0;
    {
        glGenTextures(1, &TEXTURE0);
        glActiveTexture(GL_TEXTURE0);            // -> make this texture active
        glBindTexture(GL_TEXTURE_2D, TEXTURE0);  // -> bind the texture to a texturing target

        {
            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE
            );

            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE
            );

            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR
            );

            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_MAG_FILTER,
                GL_LINEAR
            );
        }

        int width, height, nchannels;
        stbi_set_flip_vertically_on_load(GL_TRUE);
        unsigned char* data = stbi_load(
            "textures/container.jpg",
            &width,
            &height,
            &nchannels,
            0
        );

        if (!data)
            return EXIT_FAILURE;

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

        if (!shader_set_uniform1i(shader, "TEXTURE0", 0))
            return EXIT_FAILURE;
    }

    unsigned int TEXTURE1;
    {
        glGenTextures(1, &TEXTURE1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, TEXTURE1);  // this is an important step
                                                 // we switch between textures by calling this like for vao and shader program

        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // jurst reading about mipmaps and these four options to specify
            // how to switch between the mipmaps is not enough.
            glTexParameteri(
                GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR
            );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        int width, height, nchannels;
        stbi_set_flip_vertically_on_load(GL_TRUE);
        unsigned char* data = stbi_load(
            "textures/awesomeface.png",
            &width,
            &height,
            &nchannels,
            0
        );
        if (!data)
            return EXIT_FAILURE;

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        if (!shader_set_uniform1i(shader, "TEXTURE1", 1))
            return EXIT_FAILURE;
    }

    while (!window_closed(window))
    {
        window_poll_events(window);
        window_process_input(window);
        window_clear_color(window);

        {
            vec4 tv = { 0.5f, -0.5f, 0.0f };
            vec3 rv = { 0.0f, 0.0f, 1.0f };
            vec3 sv = { 0.5f, 0.5f, 0.5f };
            mat4 tm = GLM_MAT4_IDENTITY_INIT;

            glm_translate(tm, tv);
            glm_rotate(tm, (float) glfwGetTime(), rv);
            glm_scale(tm, sv);
            if (!shader_set_uniform_mat4fv(shader, "transform", (float*) tm))
                return EXIT_FAILURE;
        }
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, NULL);

        {
            vec4 tv = { -0.5f, 0.5f, 0.0f };
            vec3 rv = { 0.0f, 0.0f, 1.0f };
            vec3 sv = {
                sin(glfwGetTime()),
                sin(glfwGetTime()),
                sin(glfwGetTime()),
            };
            mat4 tm = GLM_MAT4_IDENTITY_INIT;

            glm_translate(tm, tv);
            /* glm_rotate(tm, (float) glfwGetTime(), rv); */
            /* glm_scale(tm, sv); */
            if (!shader_set_uniform_mat4fv(shader, "transform", (float*) tm))
                return EXIT_FAILURE;
        }
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, NULL);

        window_swap_buffers(window);
    }

    window_destroy(window);
    return 0;
}
