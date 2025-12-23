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

    struct Texture* textures[TEXTURE_COUNT];
    int             txCount;
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

LOADER_API void meshLoadShader(
    struct Mesh* mesh,
    const char*  vPath,
    const char*  fPath
);

LOADER_API void meshLoadTexture(
    struct Mesh*      mesh,
    const char*       path,
    const char*       shVarName,
    enum TextureIndex txIndex
);

#endif
