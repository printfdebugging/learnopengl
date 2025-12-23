#ifndef LOADER_MESH_H
#define LOADER_MESH_H

#include "glad/glad.h"

#include "loader/shader.h"
#include "loader/texture.h"

struct Mesh
{
    unsigned int vao;
    unsigned int vboVertex;
    unsigned int vboIndices;
    unsigned int vboColor;
    unsigned int vboUV;

    struct Shader* shader;
    // TODO: maybe also a count for adding multiple textures ??
    struct Texture* texture;
};

LOADER_API struct Mesh* meshCreate();
LOADER_API void         meshDestroy(struct Mesh* mesh);
LOADER_API void         meshBind(struct Mesh* mesh);
LOADER_API void         meshUnbind(struct Mesh* mesh);

// TODO: maybe provide the strides too
LOADER_API void meshLoadVertices(
    struct Mesh* mesh,
    float*       data,
    unsigned int count,
    unsigned int stride
);

LOADER_API void meshLoadIndices(
    struct Mesh* mesh,
    int*         data,
    unsigned int count
);

LOADER_API void meshLoadColors(
    struct Mesh* mesh,
    float*       data,
    unsigned int count,
    unsigned int stride
);

LOADER_API void meshLoadUV(
    struct Mesh* mesh,
    float*       data,
    unsigned int count,
    unsigned int stride
);

// LOADER_API void meshLoadShader(
//     struct Mesh* mesh,
//     const char*  vpath,
//     const char*  fpath
// );
//
// LOADER_API void meshLoadTexture(
//     struct Mesh* mesh,
//     const char*  path
// );

#endif
