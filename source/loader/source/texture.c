#include "loader/texture.h"
#include "core/logger.h"

#include "stb_image.h"

struct Texture* txLoadFromFile(const char* path,
                               const char* shaderVarName,
                               GLenum      textureNum)
{
    struct Texture* texture = malloc(sizeof(struct Texture));
    if (!texture)
    {
        ERROR("Failed to allocate memory for texture: %s\n", path);
        return NULL;
    }

    stbi_set_flip_vertically_on_load(true);

    texture->image.path    = path;
    texture->shaderVarName = shaderVarName;
    texture->image.data    = stbi_load(
        path,
        &texture->image.width,
        &texture->image.height,
        &texture->image.nrChannels,
        0
    );

    GLenum dataFormat;
    if (texture->image.nrChannels == 3)
        dataFormat = GL_RGB;
    else
        dataFormat = GL_RGBA;

    if (!texture->image.data)
    {
        ERROR("Failed to load texture %s\n", path)
        free(texture);
        return NULL;
    }

    glActiveTexture(textureNum);
    glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        dataFormat,
        texture->image.width,
        texture->image.height,
        0,
        dataFormat,
        GL_UNSIGNED_BYTE,
        texture->image.data
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    // TODO: think about it, whether we want to free the image
    // and just store it's path, or do we want to store the image as well?
    // stbi_image_free(wall);

    return texture;
}

void txDestroy(struct Texture* texture)
{
    free(texture->image.data);
    free(texture);
}
