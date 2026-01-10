#ifndef MESH_H
#define MESH_H

#include "glad/glad.h"

#include "shader.h"
#include "texture.h"

enum MeshAttribute
{
    MESH_ATTRIBUTE_POSITION = 0,
    MESH_ATTRIBUTE_COLOR    = 1,
    MESH_ATTRIBUTE_UV       = 2,
    MESH_ATTRIBUTE_COUNT,
};

struct Mesh
{
    unsigned int vao;
    unsigned int vboVertex;
    unsigned int vboColor;
    unsigned int vboUV;

    unsigned int ebo;
    unsigned int eboCount;
    GLenum       eboType;
};

struct Mesh *meshCreate();

void meshDestroy(struct Mesh *mesh);

void meshLoadVertices(
    struct Mesh *mesh,
    float       *data,
    unsigned int count,
    unsigned int stride
);

void meshLoadIndices(
    struct Mesh *mesh,
    int         *data,
    unsigned int count,
    GLenum       type
);

void meshLoadColors(
    struct Mesh *mesh,
    float       *data,
    unsigned int count,
    unsigned int stride
);

void meshLoadUV(
    struct Mesh *mesh,
    float       *data,
    unsigned int count,
    unsigned int stride
);

/*
 * returns 0 on success
 * returns 1 on failure.
 */
int meshLoadTexture(
    struct Mesh      *mesh,
    const char       *path,
    const char       *shVarName,
    enum TextureIndex txUnitIndex
);

#endif
