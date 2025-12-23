#include "core/logger.h"

#include "loader/mesh.h"
#include "loader/shader.h"

#include <stdlib.h>

struct Mesh* meshCreate()
{
    struct Mesh* mesh = malloc(sizeof(struct Mesh));
    if (!mesh)
    {
        ERROR("failed to allocate memory for mesh\n");
        return NULL;
    }

    mesh->vao       = 0;
    mesh->vboVertex = 0;
    mesh->vboColor  = 0;
    mesh->vboUV     = 0;
    mesh->shader    = NULL;
    mesh->texture   = NULL;

    glGenVertexArrays(1, &mesh->vao);

    return mesh;
}

void meshDestroy(struct Mesh* mesh)
{
    (void) mesh;
    // TODO: fill it in later
}

void meshBind(struct Mesh* mesh)
{
    glBindVertexArray(mesh->vao);
    // shBind(mesh->shader);
    // txBind(mesh->texture);
}

void meshUnbind(struct Mesh* mesh)
{
    glBindVertexArray(0);
    shUnbind(mesh->shader);
    txUnbind(mesh->texture);
}

/*
 * NOTE: if the vertices are packed together, then the stride would be
 *       the size of an individual chunk (before the attribute start repeating).
 *       also note that the last argument to `glVertexAttribPointer` is 0,
 *       that means that we are going to pass the pointer to the first data entry,
 *       not the start of the chunk.
 */
void meshLoadVertices(struct Mesh* mesh,
                      float*       data,
                      unsigned int count,
                      unsigned int stride)
{
    meshBind(mesh);
    glGenBuffers(1, &mesh->vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vboVertex);
    glBufferData(GL_ARRAY_BUFFER, count * stride * sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(MESH_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_POSITION);
}

void meshLoadIndices(struct Mesh* mesh,
                     int*         data,
                     unsigned int count)
{
    meshBind(mesh);
    glGenBuffers(1, &mesh->vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * count, data, GL_STATIC_DRAW);
}

void meshLoadColors(struct Mesh* mesh,
                    float*       data,
                    unsigned int count,
                    unsigned int stride)
{
    meshBind(mesh);
    glGenBuffers(1, &mesh->vboColor);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vboColor);
    glBufferData(GL_ARRAY_BUFFER, count * stride * sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(MESH_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_COLOR);
}

void meshLoadUV(struct Mesh* mesh,
                float*       data,
                unsigned int count,
                unsigned int stride)
{
    meshBind(mesh);
    glGenBuffers(1, &mesh->vboUV);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vboUV);
    glBufferData(GL_ARRAY_BUFFER, count * stride * sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(MESH_ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(MESH_ATTRIBUTE_UV);
}
//
// void meshLoadShader(struct Mesh* mesh,
//                     const char*  vpath,
//                     const char*  fpath)
// {
//     (void) mesh;
//     (void) vpath;
//     (void) fpath;
// }
//
// void meshLoadTexture(struct Mesh* mesh,
//                      const char*  path)
// {
//     (void) mesh;
//     (void) path;
// }
