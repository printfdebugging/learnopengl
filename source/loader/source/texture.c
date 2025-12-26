#include "loader/texture.h"
#include "core/logger.h"

#include "stb_image.h"

static GLenum txUnit[TEXTURE_COUNT] = {
    [TEXTURE0]  = GL_TEXTURE0,
    [TEXTURE1]  = GL_TEXTURE1,
    [TEXTURE2]  = GL_TEXTURE2,
    [TEXTURE3]  = GL_TEXTURE3,
    [TEXTURE4]  = GL_TEXTURE4,
    [TEXTURE5]  = GL_TEXTURE5,
    [TEXTURE6]  = GL_TEXTURE6,
    [TEXTURE7]  = GL_TEXTURE7,
    [TEXTURE8]  = GL_TEXTURE8,
    [TEXTURE9]  = GL_TEXTURE9,
    [TEXTURE10] = GL_TEXTURE10,
    [TEXTURE11] = GL_TEXTURE11,
    [TEXTURE12] = GL_TEXTURE12,
    [TEXTURE13] = GL_TEXTURE13,
    [TEXTURE14] = GL_TEXTURE14,
    [TEXTURE15] = GL_TEXTURE15,
    [TEXTURE16] = GL_TEXTURE16,
    [TEXTURE17] = GL_TEXTURE17,
    [TEXTURE18] = GL_TEXTURE18,
    [TEXTURE19] = GL_TEXTURE19,
    [TEXTURE20] = GL_TEXTURE20,
    [TEXTURE21] = GL_TEXTURE21,
    [TEXTURE22] = GL_TEXTURE22,
    [TEXTURE23] = GL_TEXTURE23,
    [TEXTURE24] = GL_TEXTURE24,
    [TEXTURE25] = GL_TEXTURE25,
    [TEXTURE26] = GL_TEXTURE26,
    [TEXTURE27] = GL_TEXTURE27,
    [TEXTURE28] = GL_TEXTURE28,
    [TEXTURE29] = GL_TEXTURE29,
    [TEXTURE30] = GL_TEXTURE30,
    [TEXTURE31] = GL_TEXTURE31,
};

int txLoadFromFile(struct Texture   *texture,
                   const char       *path,
                   const char       *shVarName,
                   enum TextureIndex txUnitIndex)
{
    texture->shVarName   = shVarName;
    texture->txUnitIndex = txUnitIndex;

    int            imgWidth;
    int            imgHeight;
    int            imgChanCount;
    unsigned char *imgData;

    stbi_set_flip_vertically_on_load(true);
    imgData = stbi_load(
        path,
        &imgWidth,
        &imgHeight,
        &imgChanCount,
        0
    );

    // TODO: make this a bit more robust
    GLenum imgFormat;
    if (imgChanCount == 3)
        imgFormat = GL_RGB;
    else
        imgFormat = GL_RGBA;

    if (!imgData)
    {
        ERROR("Failed to load texture %s\n", path)
        free(texture);
        stbi_image_free(imgData);
        return 1;
    }

    // tdoO: use the converted glenum here
    glActiveTexture(txUnit[txUnitIndex]);
    // TODO: maybe move out the texture generation/setup opengl code into
    // a static function so that it can be used by other loader functions like
    // the ones for characters etc.
    glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        imgFormat,
        imgWidth,
        imgHeight,
        0,
        imgFormat,
        GL_UNSIGNED_BYTE,
        imgData
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imgData);
    return 0;
}

struct Texture *txCreate()
{
    struct Texture *texture = malloc(sizeof(struct Texture));
    if (!texture)
    {
        ERROR("Failed to allocate memory for texture\n");
        return NULL;
    }

    *texture = (struct Texture) { 0 };
    return texture;
}

void txBind(struct Texture *texture)
{
    glActiveTexture(txUnit[texture->txUnitIndex]);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
}

void txUnbind(struct Texture *texture)
{
    /*
     * nothing to do here for now, although we can
     * do the following to be explicit (but it's wasteful)
     *
     * glActiveTexture(txUnit[texture->txIndex]);
     * glBindBuffer(GL_TEXTURE_2D, 0);
     */
    (void) texture;
}

void txDestroy(struct Texture *texture)
{
    glDeleteTextures(1, &texture->texture);
    free(texture);
}
