#ifndef TEXTURE_H
#define TEXTURE_H

#include "loader/defines.h"
#include "glad/glad.h"

// TODO: think about destroying the image later, it might be worth keeping it
// as we might want to clone a texture to create some other texture. but then
// maybe it's better to just store the path and load it again from the file
// system.
struct Texture
{
    unsigned int texture;
    const char*  shaderVarName;
    GLenum       textureNum;

    struct Image
    {
        int            width;
        int            height;
        int            nrChannels;
        unsigned char* data;
        const char*    path;
    } image;
};

LOADER_API struct Texture* txLoadFromFile(
    const char* path,
    const char* shaderVarName,
    GLenum      textureNum
);

LOADER_API void txBind(struct Texture* texture);
LOADER_API void txUnbind(struct Texture* texture);

LOADER_API void txDestroy(struct Texture* texture);

#endif
