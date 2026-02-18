#include "texture.h"
#include "logger.h"
#include "shader.h"

#include "stb_image.h"

int texture_load_from_file(struct texture *texture, const char *path) {
    int            imgWidth;
    int            imgHeight;
    int            imgChanCount;
    unsigned char *imgData;

    stbi_set_flip_vertically_on_load(true);
    imgData = stbi_load(path, &imgWidth, &imgHeight, &imgChanCount, 0);

    // TODO: make this a bit more robust using switch
    GLenum imgFormat;
    if (imgChanCount == 3)
        imgFormat = GL_RGB;
    else
        imgFormat = GL_RGBA;

    if (!imgData) {
        fprintf(stderr, "Failed to load texture %s\n", path);
        free(texture);
        stbi_image_free(imgData);
        return 1;
    }

    if (texture_load(texture, imgData, imgWidth, imgHeight, imgFormat, GL_UNSIGNED_BYTE, imgFormat, GL_TRUE))
        return 1;

    stbi_image_free(imgData);
    return 0;
}

struct texture *texture_create() {
    struct texture *texture = malloc(sizeof(struct texture));
    if (!texture) {
        fprintf(stderr, "Failed to allocate memory for texture\n");
        return NULL;
    }

    *texture = (struct texture) { 0 };
    return texture;
}

int texture_load(struct texture *texture, void *txData, unsigned int txWidth, unsigned int txHeight, GLenum txFormat, GLenum txDataType, GLenum txInternalFormat, GLboolean txGenMipmaps) {
    glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glTexImage2D(GL_TEXTURE_2D, GL_ZERO, txInternalFormat, txWidth, txHeight, GL_ZERO, txFormat, txDataType, txData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, txGenMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (txGenMipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    return 0;
}

void texture_destroy(struct texture *texture) {
    glDeleteTextures(1, &texture->texture);
    free(texture);
}
