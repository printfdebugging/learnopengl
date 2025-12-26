#include "core/logger.h"

#include "loader/mesh.h"
#include "loader/shader.h"
#include "loader/texture.h"

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
    if (mesh->vboIndices)
        glDeleteBuffers(1, &mesh->vboIndices);

    glDeleteVertexArrays(1, &mesh->vao);
}

void meshBind(struct Mesh *mesh)
{
    glBindVertexArray(mesh->vao);

    if (mesh->shader)
        shBind(mesh->shader);

    // TODO: PROBABLY use txCount intead of looping over all the values
    // here.
    for (enum TextureIndex i = TEXTURE0; i < TEXTURE_COUNT; ++i)
        if (mesh->textures[i])
            txBind(mesh->textures[i]);
}

void meshUnbind(struct Mesh *mesh)
{
    glBindVertexArray(0);
    shUnbind(mesh->shader);
    // TODO: probably we don't need to use texture
    // at all for this, so no need to loop over
    // all of them, unbind is same as vao, we just
    // bind that to zero.
    // txUnbind(mesh->texture);
    txUnbind(NULL);
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
    meshBind(mesh);
    glGenBuffers(1, &mesh->vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertex);
    glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(MESH_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_POSITION);
}

void meshLoadIndices(struct Mesh *mesh,
                     int         *data,
                     unsigned int count)
{
    meshBind(mesh);
    glGenBuffers(1, &mesh->vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * count, data, GL_STATIC_DRAW);
}

void meshLoadColors(struct Mesh *mesh,
                    float       *data,
                    unsigned int count,
                    unsigned int stride)
{
    meshBind(mesh);
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
    meshBind(mesh);
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

    shBind(mesh->shader);

    // TODO: move this to a seprate function if that
    // seems neccessary.
    for (enum TextureIndex i = TEXTURE0; i < TEXTURE_COUNT; ++i)
    {
        if (mesh->textures[i])
        {
            if (shUniform1i(
                    mesh->shader,
                    mesh->textures[i]->shVarName,
                    mesh->textures[i]->txUnitIndex
                ))
            {
                return 1;
            }
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

    mesh->textures[txUnitIndex] = txLoadFromFile(
        path,
        shVarName,
        txUnitIndex
    );

    if (!mesh->textures[txUnitIndex])
        return 1;

    mesh->txCount += 1;
    return 0;
}
