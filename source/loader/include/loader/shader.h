#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include "loader/defines.h"

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

LOADER_API struct Shader* shCreateFromFile(
    const char* vpath,
    const char* fpath
);

LOADER_API bool shUniform1i(
    const struct Shader* shader,
    const char*          name,
    int                  value
);

LOADER_API bool shUniformMatrix4fv(
    const struct Shader* shader,
    const char*          name,
    float*               value
);

#endif
