#ifndef TEXTURE_H
#define TEXTURE_H

#include "loader/defines.h"
#include "glad/glad.h"

enum TextureIndex
{
    TEXTURE0      = 0,
    TEXTURE1      = 1,
    TEXTURE2      = 2,
    TEXTURE3      = 3,
    TEXTURE4      = 4,
    TEXTURE5      = 5,
    TEXTURE6      = 6,
    TEXTURE7      = 7,
    TEXTURE8      = 8,
    TEXTURE9      = 9,
    TEXTURE10     = 10,
    TEXTURE11     = 11,
    TEXTURE12     = 12,
    TEXTURE13     = 13,
    TEXTURE14     = 14,
    TEXTURE15     = 15,
    TEXTURE16     = 16,
    TEXTURE17     = 17,
    TEXTURE18     = 18,
    TEXTURE19     = 19,
    TEXTURE20     = 20,
    TEXTURE21     = 21,
    TEXTURE22     = 22,
    TEXTURE23     = 23,
    TEXTURE24     = 24,
    TEXTURE25     = 25,
    TEXTURE26     = 26,
    TEXTURE27     = 27,
    TEXTURE28     = 28,
    TEXTURE29     = 29,
    TEXTURE30     = 30,
    TEXTURE31     = 31,
    TEXTURE_COUNT = 32,
};

struct Texture
{
    unsigned int texture;

    /*
     * Textures are used with shaders, the shaders define a `sampler2D`
     * uniform variable which refers to a texture unit on the GPU.
     * `shVarName` is the variable name this texture binds to in the
     * shader.
     */
    // TODO: this should be moved into the shader.
    const char *shVarName;

    /*
     * `txIndex` is the texture unit this texture is bound to on the GPU.
     */
    enum TextureIndex txUnitIndex;
};

LOADER_API struct Texture *txCreate();

// TODO: create enums for txFormat, txInternalFormat
LOADER_API int txLoad(
    struct Texture *texture,
    void           *txData,
    unsigned int    txWidth,
    unsigned int    txHeight,
    GLenum          txFormat,
    GLenum          txDataType,
    GLenum          txInternalFormat,
    GLboolean       txGenMipmaps
);

LOADER_API int txLoadFromFile(
    struct Texture   *texture,
    const char       *path,
    const char       *shVarName,
    enum TextureIndex txUnitIndex
);

/* TODO: later!
LOADER_API int txLoadFromData(
    struct Texture   *texture,
    const char       *path,  // <-- data, width, height, format
    const char       *shVarName,
    enum TextureIndex txIndex
);
*/

LOADER_API void txBind(struct Texture *texture);
LOADER_API void txUnbind(struct Texture *texture);
LOADER_API void txDestroy(struct Texture *texture);

#endif
