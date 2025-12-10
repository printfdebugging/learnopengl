#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

enum Attr
{
    MESH_ATTRIBUTE_POSITION = 0,
    MESH_ATTRIBUTE_COLOR    = 1,
    MESH_ATTRIBUTE_UV       = 2,
    MESH_ATTRIBUTE_COUNT,
};

struct Shader
{
    unsigned int program;
};

struct Shader* shCreateFromFile(
    const char* vpath,
    const char* fpath
);

bool shUniform1i(
    const struct Shader* shader,
    const char*          name,
    int                  value
);

bool shUniformMatrix4fv(
    const struct Shader* shader,
    const char*          name,
    float*               value
);

#endif
