#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include "loader/defines.h"

// TODO: maybe move this to mesh.h ??
enum MeshAttribute
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

LOADER_API void shDestroy(struct Shader* shader);
LOADER_API void shBind(const struct Shader* shader);
LOADER_API void shUnbind(const struct Shader* shader);

LOADER_API bool shUniform1i(
    const struct Shader* shader,
    const char*          name,
    int                  value
);

LOADER_API bool shUniform3f(
    const struct Shader* shader,
    const char*          name,
    float                first,
    float                second,
    float                third
);

LOADER_API bool shUniformMatrix4fv(
    const struct Shader* shader,
    const char*          name,
    float*               value
);

#endif
