#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "core/window.h"
#include "core/logger.h"

#include "loader/mesh.h"
#include "loader/shader.h"
#include "loader/texture.h"

#include "renderer/renderer.h"

#include <stdlib.h>

struct Character
{
    struct Texture *txGlyph;

    vec2         size;
    vec2         bearing;
    unsigned int advance;
};

struct GameData
{
    struct Window *window;
    struct Mesh   *mesh;

    struct Character characters[128];
    // maybe put glyph inside a mesh?
    // what if we create a struct font for such purposes.
    struct Shader *glyphShader;
    unsigned int   glyphVao;
    unsigned int   glyphVbo;
};

DrawFrameCallback drawFrameCallback;

void drawText(
    const char      *text,
    float            x,
    float            y,
    float            scale,
    vec3             color,
    struct GameData *data
);

int main()
{
    struct GameData data;

    data.window = winCreate(800, 600, "OpenGL", (vec4) { 0.156f, 0.172f, 0.203f, 1.0f });
    if (!data.window)
        return EXIT_FAILURE;

    /* clang-format off */
    float vertices[] = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
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

    // if (meshLoadTexture(
    //         data.mesh,
    //         (char *) ASSETS_DIR "textures/container.jpg",
    //         "containerTexture",
    //         TEXTURE0
    //     ))
    // {
    //     return EXIT_FAILURE;
    // }

    // if (meshLoadTexture(
    //         data.mesh,
    //         (char *) ASSETS_DIR "textures/awesomeface.png",
    //         "faceTexture",
    //         TEXTURE1
    //     ))
    // {
    //     return EXIT_FAILURE;
    // }

    if (meshLoadShader(
            data.mesh,
            ASSETS_DIR "shaders/shader.vert",
            ASSETS_DIR "shaders/shader.frag"
        ))
    {
        return EXIT_FAILURE;
    }

    {  // freetype loading the textures
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            ERROR("failed to initialize freetype library\n");
            return EXIT_FAILURE;
        }

        FT_Face face;
        if (FT_New_Face(ft, ASSETS_DIR "fonts/LilexNerdFont-Regular.ttf", 0, &face))
        {
            ERROR("failedt o load font\n");
            return EXIT_FAILURE;
        }

        FT_Set_Pixel_Sizes(face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glBindVertexArray(0);
        glUseProgram(0);

        for (unsigned char c = 0; c < 128; ++c)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                ERROR("failed to load glyph for character: %c\n", c);
                continue;
            }

            data.characters[c].txGlyph = txCreate();
            if (txLoad(
                    data.characters[c].txGlyph,
                    face->glyph->bitmap.buffer,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    GL_RED,
                    GL_FALSE
                ))
            {
                return -1;
            }

            // data.characters[c].txID       = texture;
            data.characters[c].size[0]    = face->glyph->bitmap.width;
            data.characters[c].size[1]    = face->glyph->bitmap.rows;
            data.characters[c].bearing[0] = face->glyph->bitmap_left;
            data.characters[c].bearing[1] = face->glyph->bitmap_top;
            data.characters[c].advance    = face->glyph->advance.x;
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }  // freetype loading the textures

    data.glyphShader = shCreate();
    if (!data.glyphShader)
        return EXIT_FAILURE;

    if (shLoadFromFile(
            data.glyphShader,
            ASSETS_DIR "shaders/glyph.vert",
            ASSETS_DIR "shaders/glyph.frag"
        ))
    {
        return EXIT_FAILURE;
    }

    {
        mat4 projection;
        // TODO: look into how the math behind this works, how to imagine these
        // these are not mere numbers, but the dimentions of a unit box at the
        // origin of the coordinate system.
        glm_ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f, projection);
        shUniformMatrix4fv(data.glyphShader, "projection", (float *) projection);
    }

    {  // vao creation code
        glGenVertexArrays(1, &data.glyphVao);
        glBindVertexArray(data.glyphVao);

        glGenBuffers(1, &data.glyphVbo);
        glBindBuffer(GL_ARRAY_BUFFER, data.glyphVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(MESH_ATTRIBUTE_POSITION);
        glVertexAttribPointer(MESH_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }  // vao creation code

    winRegisterDrawFrameCallback(data.window, drawFrameCallback);
    winFireMainLoop(data.window, (void *) &data);

    winDestroy(data.window);
    meshDestroy(data.mesh);

    return 0;
}

void drawFrameCallback(void *data)
{
    struct GameData *d = (struct GameData *) data;

    double x, y;
    int    w, h;

    glfwGetCursorPos(d->window->window, &x, &y);
    glfwGetWindowSize(d->window->window, &w, &h);
    // double t = glfwGetTime();

    // vec2 position   = { x, y };
    vec2 dimentions = {
        [0] = 800.0f,
        [1] = 600.0f,
    };

    // shUniform2fv(d->mesh->shader, "cursorPosition", (float *) position);
    shUniform2fv(d->mesh->shader, "canvasDimentions", (float *) dimentions);
    // shUniform1f(d->mesh->shader, "canvasWidth", (float) w);
    // shUniform1f(d->mesh->shader, "canvasHeight", (float) h);
    // shUniform1f(d->mesh->shader, "time", t);

    winPollEvents(d->window);
    winProcessInput(d->window);
    winClearColor(d->window);

    // drawText("printfdebugging", 240.0f, 270.0f, 0.5f, (vec3) { 0.5, 0.8f, 0.2f }, data);
    renderMesh(d->mesh);
    // interestingly enough firing drawText calls after glDrawElements
    // doesn't draw the text over the other textures & when it does
    // when i call it before glDrawElements. When it draws, the text is
    // in drawn in boxes which don't have transparent background.

    winSwapBuffers(d->window);
    winPostFrameChecks(d->window);
}

void drawText(const char      *text,
              float            x,
              float            y,
              float            scale,
              vec3             color,
              struct GameData *data)
{
    shBind(data->glyphShader);
    glBindVertexArray(data->glyphVao);
    glActiveTexture(GL_TEXTURE0);
    // TODO: in the implementations of these functions,
    // we are asking over and over "what's the location of this".
    // we should cache that in the shader. there will be probably
    // a fixed number of these variables per shader... and they would change
    // based on the shader type??? don't know.
    shUniform3f(data->glyphShader, "glyphColor", color[0], color[1], color[2]);
    shUniform1i(data->glyphShader, "glyphTexture", TEXTURE0);

    int len = strlen(text);

    for (int i = 0; i < len; ++i)
    {
        // struct Character *ch = &data->characters[i]; <-- this is wrong
        // this is rudamentary approach, later we will probably have some codepoints
        // to refer to the characters with.

        // NOTE: thanks to this, the textures (glyphs) are being sent to the
        // shaders, but still they aren't showing up, why?
        struct Character *ch = &data->characters[(unsigned int) text[i]];

        float xpos = x + ch->bearing[0] * scale;
        float ypos = y - (ch->size[1] - ch->bearing[1]) * scale;
        float w    = ch->size[0] * scale;
        float h    = ch->size[1] * scale;

        // note that the glyph texture is inverted (mirrored over x axis)
        // therefore these texture coordinates are like this
        float vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 0.0f }, // topLeft
            { xpos,     ypos,     0.0f, 1.0f }, // orogin
            { xpos + w, ypos,     1.0f, 1.0f }, // bottomRight

            { xpos,     ypos + h, 0.0f, 0.0f }, // topLeft
            { xpos + w, ypos,     1.0f, 1.0f }, // bottomRight
            { xpos + w, ypos + h, 1.0f, 0.0f }, // topRight
        };

        // so basically all text shares the same vao and same vbo,
        // irrespective of scale. and we need to save the vbo refernece
        // too as we need to dynamically change the data on it.

        glBindTexture(GL_TEXTURE_2D, ch->txGlyph->texture);
        glBindBuffer(GL_ARRAY_BUFFER, data->glyphVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch->advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
