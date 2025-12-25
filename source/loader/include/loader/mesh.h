#ifndef LOADER_MESH_H
#define LOADER_MESH_H

#include "glad/glad.h"

#include "loader/shader.h"
#include "loader/texture.h"

enum MeshAttribute
{
    MESH_ATTRIBUTE_POSITION = 0,
    MESH_ATTRIBUTE_COLOR    = 1,
    MESH_ATTRIBUTE_UV       = 2,
    MESH_ATTRIBUTE_COUNT,
};

// TODO: in my own implementation of cglm some day, use
// structs with x,y,z,union{w,a...} for vectors etc..
// that way it will be very convenient to copy them
// by assignment.

struct Mesh
{
    unsigned int vao;
    unsigned int vboVertex;
    unsigned int vboIndices;
    unsigned int vboColor;
    unsigned int vboUV;

    struct Shader *shader;

    struct Texture *textures[TEXTURE_COUNT];
    int             txCount;
};

// TODO: use this pattern, two functions to create and destroy,
// rest other functions take in the struct pointer and return
// integer for success or failure, this way, we can wrap them
// in if conditioanls.
// DO it in texture & shader.
LOADER_API struct Mesh *meshCreate();
LOADER_API void         meshDestroy(struct Mesh *mesh);
LOADER_API void         meshBind(struct Mesh *mesh);
LOADER_API void         meshUnbind(struct Mesh *mesh);

LOADER_API void meshLoadVertices(
    struct Mesh *mesh,
    float       *data,
    unsigned int count,
    unsigned int stride
);

LOADER_API void meshLoadIndices(
    struct Mesh *mesh,
    int         *data,
    unsigned int count
);

LOADER_API void meshLoadColors(
    struct Mesh *mesh,
    float       *data,
    unsigned int count,
    unsigned int stride
);

LOADER_API void meshLoadUV(
    struct Mesh *mesh,
    float       *data,
    unsigned int count,
    unsigned int stride
);

/*
 * returns 0 on success
 * returns 1 on failure.
 */
LOADER_API int meshLoadShader(
    struct Mesh *mesh,
    const char  *vPath,
    const char  *fPath
);

/*
 * returns 0 on success
 * returns 1 on failure.
 */
LOADER_API int meshLoadTexture(
    struct Mesh      *mesh,
    const char       *path,
    const char       *shVarName,
    enum TextureIndex txIndex
);

#endif
