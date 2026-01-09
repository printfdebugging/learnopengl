#include "logger.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

#include <stdlib.h>

struct Mesh *meshCreate()
{
    struct Mesh *mesh = malloc(sizeof(struct Mesh));
    if (!mesh)
    {
        ERROR("failed to allocate memory for mesh\n");
        return NULL;
    }

    *mesh = (struct Mesh) { 0 };
    glGenVertexArrays(1, &mesh->vao);
    return mesh;
}

void meshDestroy(struct Mesh *mesh)
{
    // TODO: destroy the texture
    // TODO: destroy the shader

    if (mesh->vboVertex)
        glDeleteBuffers(1, &mesh->vboVertex);
    if (mesh->vboColor)
        glDeleteBuffers(1, &mesh->vboColor);
    if (mesh->vboUV)
        glDeleteBuffers(1, &mesh->vboUV);
    if (mesh->ebo)
        glDeleteBuffers(1, &mesh->ebo);

    glDeleteVertexArrays(1, &mesh->vao);
}

/*
 * NOTE: if the vertices are packed together, then the stride would be
 *       the size of an individual chunk (before the attribute start repeating).
 *       also note that the last argument to `glVertexAttribPointer` is 0,
 *       that means that we are going to pass the pointer to the first data entry,
 *       not the start of the chunk.
 */
void meshLoadVertices(struct Mesh *mesh,
                      float       *data,
                      unsigned int count,
                      unsigned int stride)
{
    glBindVertexArray(mesh->vao);
    glGenBuffers(1, &mesh->vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertex);
    glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(MESH_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_POSITION);
}

void meshLoadIndices(struct Mesh *mesh,
                     int         *data,
                     unsigned int count,
                     GLenum       type)
{
    glBindVertexArray(mesh->vao);
    glGenBuffers(1, &mesh->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * count, data, GL_STATIC_DRAW);

    mesh->eboCount = count;
    mesh->eboType  = type;
}

void meshLoadColors(struct Mesh *mesh,
                    float       *data,
                    unsigned int count,
                    unsigned int stride)
{
    glBindVertexArray(mesh->vao);
    glGenBuffers(1, &mesh->vboColor);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vboColor);
    glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(MESH_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_COLOR);
}

void meshLoadUV(struct Mesh *mesh,
                float       *data,
                unsigned int count,
                unsigned int stride)
{
    glBindVertexArray(mesh->vao);
    glGenBuffers(1, &mesh->vboUV);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vboUV);
    glBufferData(GL_ARRAY_BUFFER, count * 2 * sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(MESH_ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_UV);
}

int meshLoadShader(struct Mesh *mesh,
                   const char  *vPath,
                   const char  *fPath)
{
    mesh->shader = shCreate();
    if (!mesh->shader)
        return 1;
    if (shLoadFromFile(mesh->shader, vPath, fPath))
        return 1;

    glUseProgram(mesh->shader->program);

    // TODO: move this to a seprate function if that
    // seems neccessary.
    for (enum TextureIndex i = TEXTURE0; i < TEXTURE_COUNT; ++i)
    {
        if (mesh->textures[i])
        {
            int location = shGetUniformLocation(mesh->shader, mesh->textures[i]->shVarName);
            if (location != -1)
                glUniform1i(location, mesh->textures[i]->txUnitIndex);
        }
    }
    return 0;
}

int meshLoadTexture(struct Mesh      *mesh,
                    const char       *path,
                    const char       *shVarName,
                    enum TextureIndex txUnitIndex)
{
    if (txUnitIndex >= TEXTURE_COUNT)
    {
        ERROR("index greater than TEXTURE_COUNT - 1: %i\n", (int) txUnitIndex);
        return 1;
    }

    mesh->textures[txUnitIndex] = txCreate();
    if (!mesh->textures[txUnitIndex])
        return 1;

    if (txLoadFromFile(mesh->textures[txUnitIndex], path, shVarName, txUnitIndex))
        return 1;

    mesh->txCount += 1;
    return 0;
}
